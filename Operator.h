#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>

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
  }

  shared_ptr<InterResult> join(shared_ptr<InterResult> src1, shared_ptr<InterResult> src2, const string &name1, const string &name2) {
    return nullptr;
  }

  shared_ptr<InterResult> contains() {
    return nullptr;
  }
}

#endif 
