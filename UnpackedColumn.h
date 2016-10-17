#ifndef UNPACKEDCOLUMN_H
#define UNPACKEDCOLUMN_H

#include <set>
#include <cmath>
#include <string>
#include <iomanip>
#include "BitPacker.h"
#include "Column.h"

template<typename T>
class UnpackedColumn : public Column
{
public:
  UnpackedColumn (const string& name, function<T(string)> parser) : Column(name), mParser(parser) {
  }

  ~UnpackedColumn() {
  }

  void addValue(const T &value) {
  };

  int getCardinality() { 
    return 0;
  };

  void endAddingValues(int recordCount) {
  }

  void insertValue(const string &value) {
    T converted = mParser(value);
    mList.push_back(converted);
  }  

  uint loadValue(uint index) {
    return mList[index];
  }

  int findIndex(const T &value) { 
    throw;
  }

  bool isValueAtIndexLessThan(const uint index, const int value) {
    return mList[index] < value;
  }

  bool isValueAtIndexGreaterThan(const uint index, const int value) {
    return mList[index] > value;
  }

  const T &getValue(const uint index) {
    return mList[index];
  }

  void printInfo() {
    cout << "Name: " << mName << endl;
    cout << "Memory for unpacked list: " << fixed << setprecision(3) << (float)sizeof(T) * mList.capacity()  / 1024 / 1024 << "MBs" << endl;
    cout << endl;
  }

  bool isPacked() {return false;}
  
private:
  function<T(const string&)> mParser;
  vector<T> mList;
};

#endif 
