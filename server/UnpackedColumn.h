#ifndef UNPACKEDCOLUMN_H
#define UNPACKEDCOLUMN_H

#include <set>
#include <cmath>
#include <string>
#include <iomanip>
#include <vector>
#include <functional>
#include <memory>

#include "BitPacker.h"
#include "TypedColumn.h"

using namespace std;

template<class T>
class UnpackedColumn : public TypedColumn<T>
{
public:
  UnpackedColumn (const string& name, function<T(string)> parser, function<string(T)> stringify) 
    : TypedColumn<T>(name), mParser(parser), mStringify(stringify){
  }

  shared_ptr<Column> clone() {
    return shared_ptr<Column>(new UnpackedColumn(this -> getName(), mParser, mStringify));
  }

  void addValue(const T &value) {};

  int getCardinality() { return 0; };

  void endAddingValues(int recordCount) {  }

  void insertValue(const string &value) {
//    cerr << 'a';
    T converted = mParser(value);
//    cerr << 's' << mList.size() << converted << endl;
    mList.push_back(converted);
//    cerr << 'd';
  }  

  string getStringValue(const uint index) {
    return mStringify(mList[index]);
  }

  bool isValueAtIndexLessThan(const uint index, const T &value) {
    return mList[index] < value;
  }

  bool isValueAtIndexGreaterThan(const uint index, const T &value) {
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
  function<string(T)> mStringify;
  vector<T> mList;
};

#endif 
