#ifndef COLUMN_H
#define COLUMN_H

#include <iostream>
#include <string>

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
  virtual void printInfo() {};

protected:
  string mName;
};

#endif
