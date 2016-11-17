#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <string>
#include <typeinfo>
#include <memory>

using namespace std;

class Column
{
public:
  Column() {};
  virtual ~Column() {};
  virtual shared_ptr<Column> clone() {};
  virtual const string &getName(){};
  virtual void addValue(const string & value) {};
  virtual string getStringValue(const uint index) {}
  virtual void insertValue(const string & value) {};
  virtual void endAddingValues(int recordCount) {};

  virtual void printInfo() {};
  virtual bool isPacked() {};

  static const bool IsString(shared_ptr<Column> c);
  static const bool IsInt(shared_ptr<Column> c);
};

#endif
