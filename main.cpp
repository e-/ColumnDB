#include <iostream>
#include <fstream>
#include <string>

#include "ColumnTable.h"
#include "Column.h"
#include "BitPacker.h"

using namespace std;

int main(int argc, char *argv[]) {
  if(argc == 1) { 
    cout << "Please specify the data file" << endl;
    return 1;
  }

  ColumnTable columnTable("Test Database");
  
  columnTable.addColumn(new TypedColumn<int>("o_orderkey"));
  columnTable.addColumn(new TypedColumn<string>("o_orderstatus"));
  columnTable.addColumn(new TypedColumn<int>("o_totalprice"));
  columnTable.addColumn(new TypedColumn<string>("o_comment"));

  columnTable.loadCSV(argv[1]);

  cout << columnTable.getRowCount() << " rows are loaded." << endl;

  cout << endl << "Enter the query (e.g., 5678 < o_totalprice < 56789)" << endl;
  cout << "Available columns: o_orderkey, o_totalprice" << endl;
  cout << "Enter \"exit\" to exit." << endl << endl;

  string query;
  cout << "Query> ";
  while(getline(cin, query))
  {
    if(query == "exit") 
      break;
    else 
    {
      columnTable.processQuery(query);
    }
    cout << "Query> ";
  }

  return 0;
}
