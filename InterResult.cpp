#include "InterResult.h"

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
/*        if(Column::IsPackedString(column)) {
          cout << ((PackedColumn<string> *)column) -> getValue(mRowIndices[j][i]) << ',';
        }
        else if(Column::IsUnpackedString(column)) {
          cout << ((UnpackedColumn<string> *)column) -> getValue(mRowIndices[j][i]) << ',';
        }
        else if(Column::IsPackedInt(column)){
          cout << ((PackedColumn<int> *)column) -> getValue(mRowIndices[j][i]) << ',';
        }
        else 
        {
          cout << ((UnpackedColumn<int> *)column) -> getValue(mRowIndices[j][i]) << ',';
        }*/
      }
    }
    cout << endl;
  }
}

void InterResult::joinSchema(shared_ptr<InterResult> src) {
  int index = 0;
  for(auto &columnTable : src -> mTables) {
    mTables.push_back(columnTable);
    mColumns.push_back(src -> mColumns[index++]);
    mRowIndices.push_back(vector<uint>(0));
  }
}
