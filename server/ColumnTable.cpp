#include "ColumnTable.h"
#include <fstream>

string stringJoin(const vector<string> &fields) {
  stringstream ss;
  for(size_t i = 0; i < fields.size(); ++i)
  {
    if(i != 0)
      ss << ",";
    ss << fields[i];
  }
  return ss.str();
}

void split(const string &s, char delim, vector<string> &elems) {
  stringstream ss;
  ss.str(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

vector<string> split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, elems);
  return elems;
}

ColumnTable::ColumnTable(const string& name) {
  this -> mName = name;
}

void ColumnTable::enableLogging(const string& logPath, bool recovery) {
  if(recovery) {
    ifstream logFile(logPath);
    string log;

    while(getline(logFile, log)) {
      vector<string> args = split(log, ',');
      if(args[0] == "insert") {
        vector<string> p(args.end() - mColumns.size(), args.end());

        insert(p);
      }
      else if(args[0] == "update") {
        vector<string> p(args.end() - mColumns.size(), args.end());
        update(p[0], p);
      }
    }
  }

  shared_ptr<LogManager> lm(new LogManager(logPath, recovery));
  mLogManager = lm;
}


shared_ptr<InterResult> ColumnTable::convertToInterResult() {
  shared_ptr<InterResult> res(new InterResult(this));

  return res;
}

void ColumnTable::loadCSV(const char * path, const char field_terminator) {
  Timer timer;
  timer.start();
//	const char field_terminator = ',';
	const char line_terminator  = '\n';
	const char enclosure_char   = '"';
  int rowCount = 0;
  int size = mColumns.size();

	csv_parser file_parser;

	file_parser.init(path);

	file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_OPTIONAL);
	file_parser.set_field_term_char(field_terminator);
	file_parser.set_line_term_char(line_terminator);

	while(file_parser.has_more_rows())
	{
		unsigned int i = 0;

		csv_row row = file_parser.get_row();

		for (i = 0; i < row.size() && i < size; i++)
		{
      mColumns[i]->addValue(row[i]);
		}
    rowCount++;
  }

  mRowCount = rowCount;

  for(auto c : mColumns) {
    c -> endAddingValues(rowCount);
  }
  
  cout << timer.end() << "s elapsed for building the dictionary" << endl;

  timer.start();
	
  file_parser.init(path);
	while(file_parser.has_more_rows())
	{
		unsigned int i = 0;

		csv_row row = file_parser.get_row();

		for (i = 0; i < row.size() && i < size; i++)
		{
      mColumns[i] -> insertValue(row[i]);
		}
    rowCount++;
  }

  cout << timer.end() << "s elapsed for bitpacking" << endl;
 

  for(auto &column : mColumns) {
    column -> printInfo();
  }
}

bool ColumnTable::insert(const vector<string> &fields) {
  unsigned int csn = ++mCsn;
  shared_ptr<Version> version(new Version(csn, fields));

  RowState rowState(true, version);

  if(mLogManager)
    mLogManager -> append("insert," + stringJoin(fields));
  mHash.insert({fields[0], rowState});
  
  return true;
}

bool ColumnTable::update(const string &key, const vector<string> &fields) {
  unsigned int csn = ++mCsn;
  auto kv = mHash.find(key);

  if(kv == mHash.end()) {
    return false; // no corresponding data
  }

    
  RowState &rowState = kv -> second;

  // mark the row as dirty
  rowState.mIsDirty = true;
  // create new version
  Version * v = new Version(csn, fields);
  shared_ptr<Version> version(v);

  // add it to the last
  auto curr = rowState.mVersion;

  if(!rowState.mVersion) {
    rowState.mVersion = version;

    if(mLogManager)
      mLogManager -> append("update," + stringJoin(fields));
  }
  else {
    while(curr -> mNext != nullptr) curr = curr -> mNext;

    if(mLogManager)
      mLogManager -> append("update," + stringJoin(curr -> mValues) + "," + stringJoin(fields));
    curr -> mNext = version;
  }

  return true;
}

vector<string> ColumnTable::scan(const string &key) {
  unsigned int csn = ++mCsn;
  auto kv = mHash.find(key);
  vector<string> res;

  if(kv == mHash.end()) {
    return res;
  }
  
  RowState &rowState = kv -> second;

  if(rowState.mIsDirty) {
    auto curr = rowState.mVersion;

    while(curr -> mNext != nullptr && curr -> mNext -> mCsn < csn) curr = curr -> mNext;
    return curr -> mValues;
  }
  else {
    uint index = 0;
    
    auto &keyColumn = mColumns[0];
    
    for(index = 0; index < mRowCount; ++index) {
      if(keyColumn -> getStringValue(index) == key)
        break;
    }

    if(index == mRowCount) {
      return res;
    }

    for(auto &column : mColumns) {
      res.push_back(column -> getStringValue(index));
    }
  }
  
  return res;
}

void ColumnTable::collect() {
  // maintain a row state for each inserted record even though it is garbage collected (for dirty bit checking)
 
  bool dirty = false;
  for(auto it = mHash.begin(); it != mHash.end(); it++) {
    if(it -> second.mIsDirty)
      dirty = true;
  }
  if(!dirty) {
    return;
  }
  
  vector<shared_ptr<Column>> newColumns;
  for(auto &column : mColumns) {
    newColumns.push_back(column -> clone());
  }

  int cn = mColumns.size();
  int updated = 0;
  
  uint oldRowCount = mRowCount;

  for(uint i = 0; i < oldRowCount; ++i) {
    auto key = mColumns[0] -> getStringValue(i);
    if(!mHash.find(key) -> second.mIsDirty) {
      for(int j = 0; j < cn; ++j) {
        string temp = mColumns[j] -> getStringValue(i);
        newColumns[j] -> addValue(temp);
      }
    }
    else {
      updated ++;
    }
  }
  
  for(auto it = mHash.begin(); it != mHash.end(); it++) {
    auto &key = it -> first; 
    RowState &rowState = it -> second;
    
    if(rowState.mIsDirty) {
      auto version = rowState.mVersion;
      
      // get the most recent version
      while(version -> mNext != nullptr) version = version -> mNext;
      
      // use version.mValues;
      for(int j = 0; j < cn; ++j) {
        newColumns[j] -> addValue(version -> mValues[j]);
      }

      mRowCount++;
    }    
  }
  
  mRowCount -= updated;

  for(auto &c : newColumns) {
    c -> endAddingValues(mRowCount);
  }

  // copy previously inserted values
  for(uint i = 0; i < oldRowCount; ++i) {
    auto key = mColumns[0] -> getStringValue(i);
    if(!mHash.find(key) -> second.mIsDirty) {
      for(int j = 0; j < cn; ++j) {
        string temp = mColumns[j] -> getStringValue(i);
        newColumns[j] -> insertValue(temp);
      }
    }
  }
  
  for(auto it = mHash.begin(); it != mHash.end(); it++) {
    auto &key = it -> first; 
    RowState &rowState = it -> second;
    
    if(rowState.mIsDirty) {
      auto version = rowState.mVersion;
      
      // get the most recent version
      while(version -> mNext != nullptr) version = version -> mNext;
      
      // use version.mValues;
      for(int j = 0; j < cn; ++j) {
        newColumns[j] -> insertValue(version -> mValues[j]);
      }

      rowState.mVersion = nullptr;
      rowState.mIsDirty = false;
    }    
  }
  
  mColumns = newColumns;

  for(auto &column : mColumns) {
    column -> printInfo();
  }
}
