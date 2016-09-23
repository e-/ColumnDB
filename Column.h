#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <string>
#include <set>
using namespace std;

class Column
{
public:
  Column(const string& name) : mName(name) {};
  virtual ~Column();
  string getName(){return mName;};
  virtual void endAddingValues() {};

private:
  string mName;
};

Column::~Column() {

}

template<typename T>
class TypedColumn : public Column
{
public:
  TypedColumn (const string& name) : Column(name) {mValues = new set<T>();};
  ~TypedColumn();
  void addValue(const T &value) { mValues->insert(value); };
  int getCardinality() { return mValues->size(); };
  void endAddingValues() {
    mDict = vector<T>(mValues->begin(), mValues->end());
    delete mValues;
  }
  
  int findIndex(const T &value) { return lower_bound(mDict.begin(), mDict.end(), value) - mDict.begin(); }

private:
  set<T> *mValues;
  vector<T> mDict;
};

template<typename T>
TypedColumn<T>::~TypedColumn(){
  
}

/*
template<typename T>
void TypedColumn<T>::addValue(const T& value) {
  mValues.insert(value);
}*/
#endif
