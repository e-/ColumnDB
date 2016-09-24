#ifndef COLUMNTABLE_H
#define COLUMNTABLE_H

#include <string>
#include <vector>
#include <cctype>
#define CSV_IO_NO_THREAD
#include "lib/csv.h"

#include "Column.h"

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
    isSelecting = true;
    selected = new vector<uint>;
    for(uint i = 0; i < mRowCount; ++i) {
      selected -> push_back(i);
    }
  };

  void select(const string& name, const string& op, const int& limit) {
    vector<uint> *temp = new vector<uint>;
    Column *column = getColumnByName(name);
    TypedColumn<int> *typedColumn = (TypedColumn<int> *)column;

    int encoded = typedColumn -> findIndex(limit);
   
    for(uint index : *selected) {
      uint value = typedColumn -> loadValue(index);
      if(op == "<" && value < encoded || op == ">" && value > encoded) {
        temp -> push_back(index);
      }
    }
    
    delete selected;
    selected = temp;
  }

  int endSelection() {
    isSelecting = false;
    int count = selected -> size();
    delete selected;
    return count;
  }

private:
  string mName;
  int mRowCount;
  vector<Column *> mColumnList;
  bool isSelecting;
  vector<uint> *selected;
};


void ColumnTable::loadCSV(const char * path) {
  io::CSVReader<4, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(path);
  in.set_header("OrderKey", "OrderStatus", "TotalPrice", "Comment");

  int orderKey, totalPrice;
  string orderStatus, comment;
  
  int rowCount = 0;

  while(in.read_row(orderKey, orderStatus, totalPrice, comment)){ 
    rowCount++;
    ((TypedColumn<decltype(orderKey)> *)mColumnList[0])->addValue(orderKey);
    ((TypedColumn<decltype(orderStatus)> *)mColumnList[1])->addValue(orderStatus);
    ((TypedColumn<decltype(totalPrice)> *)mColumnList[2])->addValue(totalPrice);
    ((TypedColumn<decltype(comment)> *)mColumnList[3])->addValue(comment);
  }
 
  mRowCount = rowCount;

  for(auto c : mColumnList) {
    c -> endAddingValues(rowCount);
  }

  io::CSVReader<4, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in2(path);
  in.set_header("OrderKey", "OrderStatus", "TotalPrice", "Comment");

  while(in2.read_row(orderKey, orderStatus, totalPrice, comment)){ 
    ((TypedColumn<int> *)mColumnList[0])->insertValue(orderKey);
    ((TypedColumn<string> *)mColumnList[1])->insertValue(orderStatus);
    ((TypedColumn<int> *)mColumnList[2])->insertValue(totalPrice);
    ((TypedColumn<string> *)mColumnList[3])->insertValue(comment);
  }
 

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
