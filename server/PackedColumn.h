#ifndef PACKEDCOLUMN_H
#define PACKEDCOLUMN_H

#include <set>
#include <cmath>
#include <string>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include "BitPacker.h"
#include "TypedColumn.h"

using namespace std;

template<class T>
class PackedColumn : public TypedColumn<T>
{
public:
  PackedColumn (const string& name, function<T(string)> parser, function<string(T)> stringify) 
    : TypedColumn<T>(name), mParser(parser), mStringify(stringify){
    mIndex = 0;
  }

  shared_ptr<Column> clone() {
    return shared_ptr<Column>(new PackedColumn(this -> getName(), mParser, mStringify));
  }

  void addValue(const string &value) {
    T converted = mParser(value);
    mValues.insert(converted);
  }
  
  int getCardinality() { return mValues.size(); };

  void endAddingValues(int recordCount) {
    mDict = vector<T>(mValues.begin(), mValues.end());
    mRecordWidth = ceil(log2(mDict.size()));
    mRecordCount = recordCount;
    mBitPacker = shared_ptr<BitPacker>(new BitPacker(mRecordWidth, mRecordCount));
  }

  void insertValue(const string &value) {
    T converted = mParser(value);

    mBitPacker -> store(mIndex++, findIndex(converted));
  }  

  string getStringValue(const uint index) {
    return mStringify(mDict[mBitPacker -> load(index)]); 
  }

  int findIndex(const T &value) { return lower_bound(mDict.begin(), mDict.end(), value) - mDict.begin(); }
  
  bool isValueAtIndexLessThan(const uint index, const T & value) {
    uint lower = findValue(value);
    return (mBitPacker -> load(index)) < lower;
  }

  bool isValueAtIndexGreaterThan(const uint index, const T & value) {
    uint lower = findValue(value);
    return (mBitPacker -> load(index)) > lower;
  }

  const T &getValue(const uint index) {
    return mDict[mBitPacker -> load(index)];
  }

  void printInfo() {
    cout << "Name: " << this -> getName() << endl;
    cout << "Cardinality: " << mDict.size() << endl;
    cout << "# of bits per value: " << mRecordWidth << endl;
    cout << "Memory for bitpacking: " << fixed << setprecision(3) << (float)mBitPacker -> getMemorySize() / 1024 / 1024 << "MBs" << endl; 
    cout << "Memory for dictionary: " << fixed << setprecision(3) << (float)sizeof(T) * mDict.capacity()  / 1024 / 1024 << "MBs" << endl;
    cout << endl;
  }
   
  bool isPacked() {return true;}
  

private:
  set<T> mValues;
  vector<T> mDict;
  shared_ptr<BitPacker> mBitPacker;
  function<T(const string&)> mParser;
  function<string(T)> mStringify;

  int mRecordWidth; // in bits
  int mRecordCount;
  int mIndex;
  T mRecentComparedValue;
  uint mRecentLowerBound, mRecentUpperBound;

  uint findValue(T value) {
    if(mRecentComparedValue != value) {
      mRecentComparedValue = value;
      mRecentLowerBound = lower_bound(mDict.begin(), mDict.end(), value) - mDict.begin();
    }

    return mRecentLowerBound;
  }
};

#endif 
