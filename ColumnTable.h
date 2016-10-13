#ifndef COLUMNTABLE_H
#define COLUMNTABLE_H

#include <string>
#include <vector>
#include <cctype>
#include "lib/csv_parser.hpp"

#include "Column.h"
#include "PackedColumn.h"
#include "UnpackedColumn.h"
#include "Timer.h"

using namespace std;

class ColumnTable
{
public:
  ColumnTable(const string& name) : mName(name) {};

  virtual ~ColumnTable() {};

  string getName() {
    return mName;
  }

  int getRowCount() {return mRowCount;}

  void addColumn(Column *column) {
    mColumnList.push_back(column);
  }

  void loadCSV(const char *path);

  void processQuery(const string &query);

  bool isValidColumnName(const string &name) {
    for(auto &column : mColumnList) {
      if(name == (*column).getName()) {
        return true;
      }
    }  
    return false;
  }

  Column *getColumnByName(const string &name) {
    for(auto &column : mColumnList) {
      if(name == (*column).getName()) {
        return column;
      }
    }
    return nullptr;
  }

  void beginSelection() {
    mIsSelecting = true;
    mSelected = new vector<uint>;
    for(uint i = 0; i < mRowCount; ++i) {
      mSelected -> push_back(i);
    }
  };

  void select(const string& name, const string& op, const int& limit) {
    vector<uint> *temp = new vector<uint>;
    Column *column = getColumnByName(name);
    PackedColumn<int> *packedColumn = (PackedColumn<int> *)column;

    int encoded = packedColumn -> findIndex(limit);
   
    for(uint index : *mSelected) {
      uint value = packedColumn -> loadValue(index);
      if(op == "<" && value < encoded || op == ">" && value > encoded) {
        temp -> push_back(index);
      }
    }
    
    delete mSelected;
    mSelected = temp;
  }

  int endSelection() {
    mIsSelecting = false;
    int count = mSelected -> size();
    delete mSelected;
    return count;
  }

private:
  string mName;
  int mRowCount;
  vector<Column *> mColumnList;
  bool mIsSelecting;
  vector<uint> *mSelected;
};


void ColumnTable::loadCSV(const char * path) {
  Timer timer;
  timer.start();
	const char field_terminator = ',';
	const char line_terminator  = '\n';
	const char enclosure_char   = '"';
  int rowCount = 0;
  int size = mColumnList.size();

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
      mColumnList[i]->addValue(row[i]);
		}
    rowCount++;
  }

  mRowCount = rowCount;

  for(auto c : mColumnList) {
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
      mColumnList[i] -> insertValue(row[i]);
		}
    rowCount++;
  }

  cout << timer.end() << "s elapsed for bitpacking" << endl;
 

  for(auto &column : mColumnList) {
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

#endif
