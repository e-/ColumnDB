#ifndef COLUMNTABLE_H
#define COLUMNTABLE_H

#include <string>
#include <vector>
#include <cctype>
#include <memory>
#include <map>

#include "lib/csv_parser.hpp"

#include "Column.h"
#include "PackedColumn.h"
#include "UnpackedColumn.h"
#include "Timer.h"

using namespace std;

class InterResult;

class RowState {
public:
  RowState(bool isDirty, unsigned int versionPointer) : mIsDirty(isDirty), mVersionPointer(versionPointer) {};

  bool mIsDirty = true;
  unsigned int mVersionPointer;

  bool mIsDeleted = false;
  unsigned int mDeletedCsn;
};

class Version {
public:
  Version(unsigned int csn, const vector<string> &values): mCsn(csn), mValues(values) {};

  unsigned int mCsn;
  vector<string> mValues;
  int mNext = -1;
};

class ColumnTable
{
  friend class InterResult;
public:
  ColumnTable(const string& name) : mName(name) {};

  virtual ~ColumnTable() {};

  string getName() {
    return mName;
  }

  int getRowCount() {return mRowCount;}

  void addColumn(Column *column) {
    mColumns.push_back(column);
  }

  void loadCSV(const char *path, const char terminator = ',');

  bool isValidColumnName(const string &name) {
    for(auto &column : mColumns) {
      if(name == column -> getName()) {
        return true;
      }
    }  
    return false;
  }

  Column *getColumnByName(const string &name) {
    for(auto &column : mColumns) {
      if(name == (*column).getName()) {
        return column;
      }
    }
    return nullptr;
  }

  shared_ptr<InterResult> convertToInterResult();
  
  bool insert(const vector<string> &fields);
  bool update(const string &key, const vector<string> &fields);
  
private:
  string mName;
  int mRowCount;
  vector<Column *> mColumns;
  map<string, RowState> mHash;
  unsigned int mCsn = 0;
  vector<Version> mVersions;
};


#endif
