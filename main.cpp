#include <iostream>
#include <fstream>
#include <string>

#include "ColumnTable.h"
#include "BitPacker.h"
#include "Timer.h"
#include "PackedColumn.h"
#include "UnpackedColumn.h"
#include "TextColumn.h"
#include "Operator.h"
#include "InterResult.h"

using namespace std;

int intParser(const string &s) {
  return stoi(s);
}

string stringParser(const string &s) {
  return string(s);
}

int main(int argc, char *argv[]) {
  if(argc == 1) { 
    cout << "Please specify the two data file" << endl;
    return 1;
  }

  ColumnTable columnTable1("Test Database");
  
  columnTable1.addColumn(new UnpackedColumn<int>("o_orderkey", intParser));
  columnTable1.addColumn(new PackedColumn<string>("o_orderstatus", stringParser));
  columnTable1.addColumn(new PackedColumn<int>("o_totalprice", intParser));
  columnTable1.addColumn(new TextColumn("o_comment"));

  Timer timer;

  timer.start();
  columnTable1.loadCSV(argv[1]);

  cout << columnTable1.getRowCount() << " rows are loaded for " <<  columnTable1.getName()<< endl;
  cout << timer.end() << "s elapsed for loading" << endl;
  cout << endl << endl;

  ColumnTable columnTable2("Test Database2");
  
  columnTable2.addColumn(new UnpackedColumn<int>("l_orderkey", intParser));
  columnTable2.addColumn(new PackedColumn<int>("l_quantity", intParser));
  columnTable2.addColumn(new PackedColumn<string>("l_returnflag", stringParser));

  timer.start();
  columnTable2.loadCSV(argv[2], '|');

  cout << columnTable2.getRowCount() << " rows are loaded for " << columnTable2.getName() << endl;
  cout << timer.end() << "s elapsed for loading" << endl;


  cout << endl << "Enter the query (e.g., 5678 < o_totalprice < 56789)" << endl;
  cout << "Available columns: o_orderkey, o_totalprice" << endl;
  cout << "Enter \"exit\" to exit." << endl << endl;

  // for testing
  //columnTable1.processQuery("o_totalprice > 5000");
  auto res1 = Op::where(columnTable1.convertToInterResult(), "o_totalprice", Op::GT, 5000);
  cout <<  res1 -> getRowCount() << " rows are found" << endl;
  res1 -> show();
  auto res2 = Op::where(columnTable1.convertToInterResult(), "o_totalprice", Op::LT, 5000);
  cout <<  res2 -> getRowCount() << " rows are found" << endl;


  //columnTable1.processQuery("o_orderkey < 10000");
  cout << Op::where(columnTable1.convertToInterResult(), "o_orderkey", Op::LT, 5000) -> getRowCount() << " rows are found" << endl;
  cout << Op::where(columnTable1.convertToInterResult(), "o_orderkey", Op::GT, 5000) -> getRowCount() << " rows are found" << endl;

  //columnTable1.processQuery("5678 < o_totalprice < 56789");
  cout << Op::where(Op::where(columnTable1.convertToInterResult(), "o_totalprice", Op::LT, 56789), "o_totalprice", Op::GT, 5678) -> getRowCount() << " rows are found" << endl;


  auto res3 = Op::join(
      columnTable1.convertToInterResult(), 
      columnTable2.convertToInterResult(), 
      "o_orderkey", 
      "l_orderkey");

  cout << res3 -> getRowCount() << " rows are found" << endl;
  
  res3 -> show();

  auto res4 = Op::join(
      Op::contains(
        columnTable1.convertToInterResult(),
        "o_comment",
        "gift"
      ), 
      columnTable2.convertToInterResult(), 
      "o_orderkey", 
      "l_orderkey"
  );

  cout << res4 -> getRowCount() << " rows are found" << endl;
  
  res4 -> show();

  return 0;
}
