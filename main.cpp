#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

#include "ColumnTable.h"
#include "Column.h"
#include "BitPacker.h"

using namespace std;

int main(int argc, char *argv[]) {
  if(argc == 1) { 
    cout << "Please specify the data file" << endl;
    return 1;
  }

/*  ColumnTable columnTable("Test Database");
  
  columnTable.addColumn(new TypedColumn<int>("o_orderkey"));
  columnTable.addColumn(new TypedColumn<string>("o_orderstatus"));
  columnTable.addColumn(new TypedColumn<int>("o_totalprice"));
  columnTable.addColumn(new TypedColumn<string>("o_comment"));

  columnTable.loadCSV(argv[1]);

  cout << columnTable.getRowCount() << " rows are loaded." << endl;
*/
/*  vector<Column *> list;
  list.push_back(new TypedColumn<int>("int"));
  list.push_back(new TypedColumn<string>("string"));
  cout << list[0]->getName() << endl;
  cout << list[1]->getName() << endl;
  cout << orderKeySet.size() << endl;
  cout << rowCount << " rows have been processed." << endl;
*/
 

  return 0;
}
