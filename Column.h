#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <string>
#include <typeinfo>

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


/*  const string &getRawStringAtIndex(const uint index) 
  {
    if(PackedColumn<string> *p = dynamic_cast<PackedColumn<string> *>(this)) {
      return p -> getRawValueAtIndex(index);
    }
    else if(UnpackedColumn<string> *p = dynamic_cast<UnpackedColumn<string> *>(this)) {
      return NULL; //true; //p -> isValueAtIndexLessThan(index, value);
    }
    return NULL; //
  }

  const int &getRawIntAtIndex(const uint index) 
  {
    if(PackedColumn<int> *p = dynamic_cast<PackedColumn<int> *>(this)) {
      return NULL;
//      return p -> getRawStringAtIndex(index);
    }
    else if(UnpackedColumn<int> *p = dynamic_cast<UnpackedColumn<int> *>(this)) {
      return NULL; //true; //p -> isValueAtIndexLessThan(index, value);
    }
    return NULL; //
  }*/

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

  static const bool IsPackedString(Column *c);
  static const bool IsUnpackedString(Column *c);
  static const bool IsPackedInt(Column *c);
  static const bool IsUnpackedInt(Column *c);

protected:
  string mName;
};

#endif
