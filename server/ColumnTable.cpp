#include "ColumnTable.h"
#include "InterResult.h"

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
  shared_ptr<Version> version(new Version(csn, fields));

  // add it to the last
  auto curr = rowState.mVersion;

  if(rowState.mVersion == nullptr) rowState.mVersion = version;
  else while(curr -> mNext != nullptr) curr = curr -> mNext;

  curr -> mNext = version;

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
  auto curr = rowState.mVersion;

  while(curr -> mNext != nullptr && curr -> mNext -> mCsn < csn) curr = curr -> mNext;

  if(curr -> mCsn < csn) {
    // load values    
  }
  
  return res;
}

void ColumnTable::collect() {
  // maintain a row state for each inserted record even though it is garbage collected (for dirty bit checking)
 
  cerr << "starting garbage collection" << endl;

  bool dirty = false;
  for(auto it = mHash.begin(); it != mHash.end(); it++) {
    if(it -> second.mIsDirty)
      dirty = true;
  }
  if(!dirty) {
    cerr << "no dirty records, exiting" << endl;
    return;
  }
  
  cerr << "creating new columns" << endl;

  vector<Column *> newColumns;
  for(auto &column : mColumns) {
    newColumns.push_back(column -> clone());
  }

  cerr << "column copied" << endl;

  // insert all current values

  int cn = mColumns.size();
  uint oldRowCount = mRowCount;
  cerr << oldRowCount << endl;

  for(uint i = 0; i < oldRowCount; ++i) {
    for(int j = 0; j < cn; ++j) {
      string temp = mColumns[j] -> getStringValue(i);
      newColumns[j] -> addValue(temp);
    }
  }
  
  cerr << "current values inserted" << endl;

  for(auto it = mHash.begin(); it != mHash.end(); it++) {
    auto &key = it -> first; 
    RowState &rowState = it -> second;
    
    if(rowState.mIsDirty) {
      auto version = rowState.mVersion;
      
      // get the most recent version
      while(version -> mNext != nullptr) version = version -> mNext;
      
      // use version.mValues;
      for(int j = 0; j < cn; ++j) {
     //   cerr << version -> mValues[j] << endl;
        newColumns[j] -> addValue(version -> mValues[j]);
      }

      mRowCount++;
    }    
  }
  
  cerr << "values added" << mRowCount << endl;

  for(auto c : newColumns) {
    c -> endAddingValues(mRowCount);
  }
  cerr << "endAddingValues" << endl;

  // copy previously inserted values
  for(uint i = 0; i < oldRowCount; ++i) {
    for(int j = 0; j < cn; ++j) {
      newColumns[j] -> insertValue(mColumns[j] -> getStringValue(i));
    }
  }
  
  cerr << "qwer" << endl;
  
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
  
  for(auto &column : mColumns)
    delete column;

  mColumns = newColumns;

  for(auto &column : mColumns) {
    column -> printInfo();
  }

  cerr << "garbage collected!" << endl;
}
