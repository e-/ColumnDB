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

  void processQuery(const string &query);

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

//    int encoded = packedColumn -> findIndex(limit);
   
    for(uint index : *mSelected) {
//      uint value = packedColumn -> loadValue(index);
      if(op == "<" && column -> isValueAtIndexLessThan(index, limit) || op == ">" && column -> isValueAtIndexGreaterThan(index, limit)) {
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
 
  shared_ptr<InterResult> convertToInterResult();

private:
  string mName;
  int mRowCount;
  vector<Column *> mColumns;
  bool mIsSelecting; // should be removed
  vector<uint> *mSelected; // should be removed
};


#endif
