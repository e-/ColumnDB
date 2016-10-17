#ifndef COLUMNTABLE_H
#define COLUMNTABLE_H

#include <string>
#include <vector>
#include <cctype>
#include <memory>

#include "lib/csv_parser.hpp"

#include "Column.h"
#include "PackedColumn.h"
#include "UnpackedColumn.h"
#include "Timer.h"

using namespace std;

class InterResult;

class ColumnTable
{
  friend class InterResult;
public:
  ColumnTable(const string& name) : mName(name) {};

  virtual ~ColumnTable() {};

  string getName() {
    return mName;
  }

  int getRowCount() {return mRowCount;}

  void addColumn(Column *column) {
    mColumns.push_back(column);
  }

  void loadCSV(const char *path, const char terminator = ',');

  bool isValidColumnName(const string &name) {
    for(auto &column : mColumns) {
      if(name == (*column).getName()) {
        return true;
      }
    }  
    return false;
  }

  Column *getColumnByName(const string &name) {
    for(auto &column : mColumns) {
      if(name == (*column).getName()) {
        return column;
      }
    }
    return nullptr;
  }

  shared_ptr<InterResult> convertToInterResult();

private:
  string mName;
  int mRowCount;
  vector<Column *> mColumns;
};


#endif
