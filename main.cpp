#include <iostream>
#include <fstream>
#include <string>

#include "ColumnTable.h"
#include "BitPacker.h"
#include "Timer.h"
#include "TypedColumn.h"
#include "UnpackedColumn.h"

using namespace std;

int intParser(const string &s) {
  return stoi(s);
}

string stringParser(const string &s) {
  return string(s);
}

int main(int argc, char *argv[]) {
  if(argc == 1) { 
    cout << "Please specify the data file" << endl;
    return 1;
  }

  ColumnTable columnTable("Test Database");
  
  columnTable.addColumn(new TypedColumn<int>("o_orderkey", intParser));
  columnTable.addColumn(new TypedColumn<string>("o_orderstatus", stringParser));
  columnTable.addColumn(new TypedColumn<int>("o_totalprice", intParser));
  columnTable.addColumn(new TypedColumn<string>("o_comment", stringParser));

  Timer timer;

  timer.start();
  columnTable.loadCSV(argv[1]);
  cout << columnTable.getRowCount() << " rows are loaded." << endl;
  cout << timer.end() << "s elapsed for loading" << endl;

  cout << endl << "Enter the query (e.g., 5678 < o_totalprice < 56789)" << endl;
  cout << "Available columns: o_orderkey, o_totalprice" << endl;
  cout << "Enter \"exit\" to exit." << endl << endl;

  string query;
  cout << "Query> ";

  // for testing
  //
  columnTable.processQuery("o_totalprice > 5000");
  columnTable.processQuery("o_orderkey < 10000");

  return 0;
  // end testing

  while(getline(cin, query))
  {
    if(query == "exit") 
      break;
    else 
    {
      timer.start();
      columnTable.processQuery(query);
      cout << timer.end() << "s elapsed for querying" << endl;
    }
    cout << "Query> ";
  }

  return 0;
}
