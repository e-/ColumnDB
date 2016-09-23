#ifndef COLUMNTABLE_H
#define COLUMNTABLE_H

#include <string>
#include <vector>
#define CSV_IO_NO_THREAD
#include "lib/csv.h"

#include "Column.h"

using namespace std;

class ColumnTable
{
public:
  ColumnTable(const string& name) : mName(name) {};
  virtual ~ColumnTable();
  string getName();
  void loadCSV(const char *path);
  int getRowCount() {return mRowCount;}
  void addColumn(Column *column);
private:
  string mName;
  int mRowCount;
  vector<Column *> mColumnList;
};

string ColumnTable::getName(){
  return mName;
}

ColumnTable::~ColumnTable() {

}

void ColumnTable::addColumn(Column *column) {
  mColumnList.push_back(column);
}

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
 
  for(auto c : mColumnList) {
    c -> endAddingValues();
  }
/*  cout << ((TypedColumn<int> *) mColumnList[0])->getCardinality() << endl;
  cout << ((TypedColumn<int> *) mColumnList[1])->getCardinality() << endl;
  cout << ((TypedColumn<string> *) mColumnList[2])->getCardinality() << endl;
  cout << ((TypedColumn<string> *) mColumnList[3])->getCardinality() << endl;
 */
  cout << ((TypedColumn<int> *)mColumnList[2]) -> findIndex(94111) << endl;


  mRowCount = rowCount;
}

#endif
