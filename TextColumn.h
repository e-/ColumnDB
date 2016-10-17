#ifndef TEXTCOLUMN_H
#define TEXTCOLUMN_H

#include <set>
#include <cmath>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <functional>
#include <sstream>
#include <cctype>

#include "BitPacker.h"
#include "TypedColumn.h"

using namespace std;

class TextColumn : public TypedColumn<string>
{
public:
  TextColumn (const string& name) : TypedColumn<string>(name) {}

  void addValue(const string &value) {};

  int getCardinality() { return 0; };

  void endAddingValues(int recordCount) {  }

  void insertValue(const string &value) {
    int index = mList.size();
    mList.push_back(value);
		string temp = value;
    for(auto &c : temp) {
      if(ispunct(c)) c = ' ';
    }	
    for(auto &token : split(temp)) {
      if(token.length() > 0) {
        mInvertedIndex[token].push_back(index);
      }
    }
  }  

  bool isValueAtIndexLessThan(const uint index, const string &value) {
    return mList[index] < value;
  }

  bool isValueAtIndexGreaterThan(const uint index, const string &value) {
    return mList[index] > value;
  }

  const string &getValue(const uint index) {
    return mList[index];
  }

  void printInfo() {
    cout << "Name: " << this -> getName() << endl;
    cout << "Memory for unpacked list: " << fixed << setprecision(3) << (float)sizeof(string) * mList.capacity()  / 1024 / 1024 << "MBs" << endl;
    cout << endl;
  }
  
  bool isPacked() {return false;}
  
  map<string, vector<int>> mInvertedIndex;

private:
  vector<string> mList;
  
	vector<string> split(const string &s, char delim = ' ') {
		stringstream ss(s);
		string item;
		vector<string> tokens;
		while (getline(ss, item, delim)) {
			tokens.push_back(item);
		}
		return tokens;
	}
};

#endif 
