#ifndef TYPEDCOLUMN_H
#define TYPEDCOLUMN_H

#include <iostream>
#include <string>
#include <typeinfo>
#include "Column.h"

using namespace std;

template <class T>
class TypedColumn : public Column
{
public:
  TypedColumn(const string& name) : mName(name) {};
  const string &getName(){return mName;}
  virtual void addValue(const string & value) {};
  virtual void insertValue(const string & value) {};
  virtual void endAddingValues(int recordCount) {};

  virtual bool isValueAtIndexLessThan(const uint index, const T & value) {};
  
  virtual bool isValueAtIndexGreaterThan(const uint index, const T & value) {};

  virtual const T &getValue(const uint index) {};

  virtual void printInfo() {};
  virtual bool isPacked() {};

protected:
  string mName;
};

#endif
