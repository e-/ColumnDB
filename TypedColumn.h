#ifndef TYPEDCOLUMN_H
#define TYPEDCOLUMN_H

#include <set>
#include <cmath>
#include <string>
#include <iomanip>
#include "BitPacker.h"
#include "Column.h"
#include <algorithm>
#include <functional>

using namespace std;

template<typename T>
class TypedColumn : public Column
{
public:
  TypedColumn (const string& name, function<T(string)> parser) : Column(name), mParser(parser) {
    mValues = new set<T>();
    mIndex = 0;
  }

  ~TypedColumn() {
    delete mBitPacker;
    delete mValues;
  }

  void addValue(const string &value) {
    T converted = mParser(value);
    mValues -> insert(converted);
  }
//  void addValue(const T &value) { mValues->insert(value); };

  int getCardinality() { return mValues->size(); };

  void endAddingValues(int recordCount) {
    mDict = vector<T>(mValues->begin(), mValues->end());
    mRecordWidth = ceil(log2(mDict.size()));
    mRecordCount = recordCount;
    mBitPacker = new BitPacker(mRecordWidth, mRecordCount);

    delete mValues;
  }

  void insertValue(const string &value) {
    T converted = mParser(value);

    mBitPacker -> store(mIndex++, findIndex(converted));
  }  

  uint loadValue(uint index) {
    return mBitPacker -> load(index);
  }

  int findIndex(const T &value) { return lower_bound(mDict.begin(), mDict.end(), value) - mDict.begin(); }

  void printInfo() {
    cout << "Name: " << mName << endl;
    cout << "Cardinality: " << mDict.size() << endl;
    cout << "# of bits per value: " << mRecordWidth << endl;
    cout << "Memory for bitpacking: " << fixed << setprecision(3) << (float)mBitPacker -> getMemorySize() / 1024 / 1024 << "MBs" << endl; 
    cout << "Memory for dictionary: " << fixed << setprecision(3) << (float)sizeof(T) * mDict.capacity()  / 1024 / 1024 << "MBs" << endl;
    cout << endl;
  }
  
private:
  set<T> *mValues;
  vector<T> mDict;
  BitPacker *mBitPacker;
  function<T(const string&)> mParser;

  int mRecordWidth; // in bits
  int mRecordCount;
  int mIndex;
};

#endif 