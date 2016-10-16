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

void ColumnTable::processQuery(const string& query) {
  vector<string> tokens;
  int temp = 0, running = 0, running2 = 0;
  string strTemp = "";

  for(auto c : query){
    if(c== '<' || c == '>') {
      if(running) {
        tokens.push_back(to_string(temp));
        running = 0;
        temp = 0;
      }
      if(running2) {
        tokens.push_back(strTemp);
        running2 = 0;
        strTemp = "";
      }
      tokens.push_back(string(1, c));
    }
    else if('0' <= c && c <= '9')
    {
      running = 1;
      temp = temp * 10 + c - '0';
    }
    else if(!isspace(c)) {
      running2 = 1;
      strTemp += string(1, c);
    }
  }
  if(running) {
    tokens.push_back(to_string(temp));
    running = 0;
    temp = 0;
  }
  if(running2) {
    tokens.push_back(strTemp);
    running2 = 0;
    strTemp = "";
  }

  try{
    beginSelection();
    for(auto x = tokens.begin(); x != tokens.end(); x++) {
      if(*x == "<" || *x == ">") {
        string &left = *(x - 1);
        string &right = *(x + 1);
        string op = *x;
        string rop = (*x == "<") ? ">" : "<";

        if(isValidColumnName(left)) {
          select(left, op, stoi(right));
        }        
        else if(isValidColumnName(right)) {
          select(right, rop, stoi(left));
        }
      }
    }
    int count = endSelection();
    cout << count << " rows are found" << endl;
  }
  catch (const exception& e)
  {
    cerr << "Invalid Query: " << e.what() << endl;
  }
}

