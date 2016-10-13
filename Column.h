#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <string>

template<typename T>
class PackedColumn;

template<typename T>
class UnpackedColumn;

using namespace std;

class Column
{
public:
  Column(const string& name) : mName(name) {};
  virtual ~Column() {};
  string getName(){return mName;};
  virtual void addValue(const string & value) {};
  virtual void insertValue(const string & value) {};
  virtual void endAddingValues(int recordCount) {};

  template<typename T>
  bool isValueAtIndexLessThan(const uint index, const T & value) 
  {
    if(this -> isPacked()) {
      return (static_cast<PackedColumn<T> *>(this)) -> isValueAtIndexLessThan(index, value);
    } else {
      return (static_cast<UnpackedColumn<T> *>(this)) -> isValueAtIndexLessThan(index, value);
    }
  }

  template<typename T>
  bool isValueAtIndexGreaterThan(const uint index, const T & value) {
    if(this -> isPacked()) {
      return (static_cast<PackedColumn<T> *>(this)) -> isValueAtIndexGreaterThan(index, value);
    } else {
      return (static_cast<UnpackedColumn<T> *>(this)) -> isValueAtIndexGreaterThan(index, value);
    }
  }

  virtual void printInfo() {};
  virtual bool isPacked() {};

protected:
  string mName;
};

#endif
