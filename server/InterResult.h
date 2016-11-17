#ifndef INTERRESULT_H
#define INTERRESULT_H

#include <iostream>
#include <string>
#include <vector>

#include "Column.h"
#include "ColumnTable.h"

using namespace std;

class InterResult
{
public:
  InterResult() = default;
  InterResult(ColumnTable *table) {
    mTables.push_back(table);
    mColumns.push_back(table -> mColumns);
   
    vector<uint> indices(table -> mRowCount);
    mRowIndices.push_back(indices);
    for(uint i = 0; i < table -> mRowCount; ++i)
      mRowIndices[0][i] = i;
  }
  
  InterResult(shared_ptr<InterResult> res) : mTables(res -> mTables), mColumns(res -> mColumns), mRowIndices(res -> mRowIndices) {}

  shared_ptr<Column> getColumnByName(const string &name) {
    for(auto &columns : mColumns) {
      for(auto &column : columns) {
        if(name == column->getName()) {
          return column;
        }
      }
    }
    return nullptr;
  }

  int findIndexByColumnName(const string &name) {
    int index = 0;
    for(auto &columns : mColumns) {
      for(auto &column : columns) {
        if(name == column->getName()) {
          return index;
        }
      }
      index++;
    }
    return -1;
  }

  int getRowCount() {
    return mRowIndices[0].size();
  }

  void clearAllRowIndices() {
    for(auto &indices : mRowIndices) {
      indices.clear();
    }
  }

  void concat(shared_ptr<InterResult> src);

  void show(int n = 10);

  vector<ColumnTable *> mTables;
  vector<vector<shared_ptr<Column>>> mColumns;
  vector<vector<uint>> mRowIndices;
};

#endif
