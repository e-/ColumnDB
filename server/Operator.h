#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
#include <map>

#include "InterResult.h"
#include "TextColumn.h"

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
          if(((TypedColumn<int> *)column) -> isValueAtIndexLessThan(targetIndex, value)) {
            break;
          }
          continue;

        case GT:
          if(((TypedColumn<int> *)column) -> isValueAtIndexGreaterThan(targetIndex, value)) {
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
    string cname1 = name1;
    string cname2 = name2;
    if(src1 -> getRowCount() < src2 -> getRowCount()) {
      auto temp = src1;
      src1 = src2;
      src2 = temp;

      auto temp2 = cname1;
      cname1 = cname2;
      cname2 = temp2;
    }

    shared_ptr<InterResult> res1(new InterResult(src1));
    shared_ptr<InterResult> res2(new InterResult(src2));
    res1 -> clearAllRowIndices();
    res2 -> clearAllRowIndices();
    
    TypedColumn<int> *column1 = (TypedColumn<int> *)src1 -> getColumnByName(cname1);
    int partialIndex1 = src1 -> findIndexByColumnName(cname1);
    TypedColumn<int> *column2 = (TypedColumn<int> *)src2 -> getColumnByName(cname2);
    int partialIndex2 = src2 -> findIndexByColumnName(cname2);

    // src1 is bigger than src2
    
    map<int, vector<int>> m;
    int sz1 = src1 -> getRowCount();
    for(int i = 0; i < sz1; ++i) {
      uint index = src1 -> mRowIndices[partialIndex1][i];
      int value = column1 -> getValue(index);
      
      m[value].push_back(i);
    }

    
    int sz2 = src2 -> getRowCount();
    for(int i = 0; i < sz2; ++i) {
      uint index2 = src2 -> mRowIndices[partialIndex2][i];
      int value2 = column2 -> getValue(index2);

      if(m.find(value2) != m.end()) {
        for(auto &index1 : m[value2]) {
          // src1 index1, src2 index2 -> join
          
          for(int j = 0; j < res1 -> mTables.size(); ++j)
            res1 -> mRowIndices[j].push_back(src1 -> mRowIndices[j][index1]);

          for(int j = 0; j < res2 -> mTables.size(); ++j)
            res2 -> mRowIndices[j].push_back(src2 -> mRowIndices[j][index2]);
        }
      }
    }


    res1 -> concat(res2);

    return res1;
  }

  // InterResult contains a single table
  shared_ptr<InterResult> contains(shared_ptr<InterResult> src, const string &name, const string &value, const bool plural) {
    shared_ptr<InterResult> res(new InterResult(src));

    TextColumn *column = (TextColumn *)res -> getColumnByName(name);
    int iri = res -> findIndexByColumnName(name);
    
    res -> clearAllRowIndices();

    for(auto &index : column -> mInvertedIndex[value]) {
      res -> mRowIndices[0].push_back(index);
    }

    if(plural) {
      for(auto &index : column -> mInvertedIndex[value + "s"]) {
        res -> mRowIndices[0].push_back(index);
      }
    }

    return res;
  }


}

#endif 
