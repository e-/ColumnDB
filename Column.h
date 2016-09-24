#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <cmath>
#include "BitPacker.h"

using namespace std;

class Column
{
public:
  Column(const string& name) : mName(name) {};
  virtual ~Column();
  string getName(){return mName;};
  virtual void endAddingValues(int recordCount) {};
  virtual void printInfo() {};

protected:
  string mName;
};

Column::~Column() {

}

template<typename T>
class TypedColumn : public Column
{
public:
  TypedColumn (const string& name) : Column(name) {
    mValues = new set<T>();
    mIndex = 0;
  };
  ~TypedColumn();
  void addValue(const T &value) { mValues->insert(value); };
  int getCardinality() { return mValues->size(); };
  void endAddingValues(int recordCount) {
    mDict = vector<T>(mValues->begin(), mValues->end());
    mRecordWidth = ceil(log2(mDict.size()));
    mRecordCount = recordCount;
    mBitPacker = new BitPacker(mRecordWidth, mRecordCount);

    delete mValues;
  }
  void insertValue(const T &value) {
    mBitPacker->store(mIndex++, findIndex(value));
  }  
  int findIndex(const T &value) { return lower_bound(mDict.begin(), mDict.end(), value) - mDict.begin(); }

  void printInfo() {
    cout << "Name: " << mName << endl;
    cout << "Cardinality: " << mDict.size() << endl;
    cout << "# of bits per value: " << mRecordWidth << endl;
    cout << "Occupied size in memory: " << fixed << setprecision(3) << (float)mBitPacker -> getMemorySize() / 1024 / 1024 << "MBs" << endl; 
    cout << endl;
  }

private:
  set<T> *mValues;
  vector<T> mDict;
  BitPacker *mBitPacker;

  int mRecordWidth; // in bits
  int mRecordCount;
  int mIndex;
};

template<typename T>
TypedColumn<T>::~TypedColumn(){ 
  delete mBitPacker;
  delete mValues;
}

/*
template<typename T>
void TypedColumn<T>::addValue(const T& value) {
  mValues.insert(value);
}*/
#endif
