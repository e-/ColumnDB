#include "InterResult.h"

InterResult::InterResult(ColumnTable *table) {
  mTables.push_back(table);
  mColumns.push_back(table -> mColumns);
 
  vector<uint> indices(table -> mRowCount);
  mRowIndices.push_back(indices);
  for(uint i = 0; i < table -> mRowCount; ++i)
    mRowIndices[0][i] = i;
}

void InterResult::show(int n) {
  if(n > mRowIndices[0].size()) n = mRowIndices[0].size();
  
  // print column headers
  for(auto &columns : mColumns) {
    for(auto &column : columns) {
      cout << column -> getName() << ',';
    }
  }
  cout << endl;
  cout << "=============================================" << endl;
  
  // print rows
  for(int i = 0; i < n; ++i) {
    for(int j = 0; j < mTables.size(); ++j) {
      for(auto &column : mColumns[j]) {
        if(Column::IsString(column)) {
          cout << ((TypedColumn<string> *)column.get()) -> getValue(mRowIndices[j][i]) << ',';
        }
        else if(Column::IsInt(column)){
          cout << ((TypedColumn<int> *)column.get()) -> getValue(mRowIndices[j][i]) << ',';
        }
      }
    }
    cout << endl;
  }
}

void InterResult::concat(shared_ptr<InterResult> src) {
  int index = 0;
  for(auto &columnTable : src -> mTables) {
    mTables.push_back(columnTable);
    mColumns.push_back(src -> mColumns[index]);
    mRowIndices.push_back(src -> mRowIndices[index]);
    index++;
  }
}

