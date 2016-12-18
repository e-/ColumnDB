#ifndef COLUMNTABLE_H
#define COLUMNTABLE_H

#include <string>
#include <vector>
#include <cctype>
#include <memory>
#include <map>
#include <mutex>
#include <sstream>

#include "lib/csv_parser.hpp"

#include "Column.h"
#include "PackedColumn.h"
#include "UnpackedColumn.h"
#include "Timer.h"
#include "LogManager.h"
#include "InterResult.h"
#include "Util.h"

using namespace std;

class InterResult;
class Version;

class Version {
public:
  Version(uint csn, const vector<string> &values): mCsn(csn), mValues(values) {};

  uint mCsn;
  vector<string> mValues;
  shared_ptr<Version> mNext;
};

class RowState {
public:
  RowState(bool isDirty, shared_ptr<Version> version) : mIsDirty(isDirty), mVersion(version) {};

  bool mIsDirty = true;
  shared_ptr<Version> mVersion = nullptr;

  bool mIsDeleted = false;
  unsigned int mDeletedCsn;
};

class ColumnTable
{
  friend class InterResult;
public:
  ColumnTable(const string& name, const string& logPath, bool recovery);
  virtual ~ColumnTable() {};

  string getName() {
    return mName;
  }

  int getRowCount() {return mRowCount;}

  void addColumn(shared_ptr<Column> column) {
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

  shared_ptr<Column> getColumnByName(const string &name) {
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
  vector<string> scan(const string &key);
  void collect();

  mutex lock;

private:
  string mName;
  uint mRowCount = 0;
  vector<shared_ptr<Column>> mColumns;
  map<string, RowState> mHash;
  unsigned int mCsn = 0;
  shared_ptr<LogManager> mLogManager;
};


#endif
