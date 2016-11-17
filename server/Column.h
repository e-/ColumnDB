#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

class Column
{
public:
  Column() {};
  virtual Column* clone() {};
  virtual const string &getName(){};
  virtual void addValue(const string & value) {};
  virtual string getStringValue(const uint index) {}
  virtual void insertValue(const string & value) {};
  virtual void endAddingValues(int recordCount) {};

  virtual void printInfo() {};
  virtual bool isPacked() {};

  static const bool IsString(Column *c);
  static const bool IsInt(Column *c);

/*  static const bool IsPackedString(Column *c);
  static const bool IsUnpackedString(Column *c);
  static const bool IsPackedInt(Column *c);
  static const bool IsUnpackedInt(Column *c);*/

};

#endif
