#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <map>

#include "InterResult.h"

namespace Op {
  enum Comp { EQ, LT, GT };

  shared_ptr<InterResult> where(shared_ptr<InterResult> src, const string &name, Comp comp, const int value) {
    shared_ptr<InterResult> res(new InterResult(src));
    Column *column = res -> getColumnByName(name);
    int iri = res -> findIndexByColumnName(name);
    
    res -> clearAllRowIndices();

    int index = -1;
    for(auto targetIndex : src -> mRowIndices[iri]) {
      index ++;
/*      switch(comp) {
        case LT:
          if(column -> isValueAtIndexLessThan(targetIndex, value)) {
            break;
          }
          continue;

        case GT:
          if(column -> isValueAtIndexGreaterThan(targetIndex, value)) {
            break;
          }
          continue;
      }
  */    
      for(int i = 0; i < src -> mTables.size(); ++i) {
        res -> mRowIndices[i].push_back(src -> mRowIndices[i][index]);
      }
    }

    return res;
  }

  shared_ptr<InterResult> join(shared_ptr<InterResult> src1, shared_ptr<InterResult> src2, const string &name1, const string &name2) {
    return nullptr;
/*    if(src1.getRowCount() > src2.getRowCount()) {
      auto temp = src1;
      src1 = src2;
      src2 = temp;
    }

    shared_ptr<InterResult> res1(new InterResult(src1));
    shared_ptr<InterResult> res2(new InterResult(src2));
    res1 -> clearAllRowIndices();
    res2 -> clearAllRowIndices();
    
    Column *column1 = res -> getColumnByName(name1);
    int index1 = src1 -> findIndexByColumnName(name1);
    Column *column2 = res -> getColumnByName(name2);
    int index2 = src2 -> findIndexByColumnName(name2);

    map<int, vector<int>> m;
    int sz1 = src1.getRowCount();
    for(int i = 0; i < sz1; ++i) {
      column1 -> 
    }
  
//    res -> joinSchema(src2);

    return res;
*/
/*    
    res -> clearAllRowIndices();

    int index = -1;
    for(auto targetIndex : src -> mRowIndices[iri]) {
      index ++;
      switch(comp) {
        case LT:
          if(column -> isValueAtIndexLessThan(targetIndex, value)) {
            break;
          }
          continue;

        case GT:
          if(column -> isValueAtIndexGreaterThan(targetIndex, value)) {
            break;
          }
          continue;
      }
      
      for(int i = 0; i < src -> mTables.size(); ++i) {
        res -> mRowIndices[i].push_back(src -> mRowIndices[i][index]);
      }
    }

    return res;

    return nullptr;*/
  }

  shared_ptr<InterResult> contains() {
    return nullptr;
  }
}

#endif 
