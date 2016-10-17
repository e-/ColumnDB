#ifndef UNPACKEDCOLUMN_H
#define UNPACKEDCOLUMN_H

#include <set>
#include <cmath>
#include <string>
#include <iomanip>
#include <vector>
#include <functional>

#include "BitPacker.h"
#include "TypedColumn.h"

using namespace std;

template<class T>
class UnpackedColumn : public TypedColumn<T>
{
public:
  UnpackedColumn (const string& name, function<T(string)> parser) : TypedColumn<T>(name), mParser(parser) {
  }

  void addValue(const T &value) {};

  int getCardinality() { return 0; };

  void endAddingValues(int recordCount) {  }

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
    cout << "Name: " << this -> getName() << endl;
    cout << "Memory for unpacked list: " << fixed << setprecision(3) << (float)sizeof(T) * mList.capacity()  / 1024 / 1024 << "MBs" << endl;
    cout << endl;
  }
  
  bool isPacked() {return false;}
  
private:
  function<T(const string&)> mParser;
  vector<T> mList;
};

#endif 
