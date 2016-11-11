#include <iostream>
#include <fstream>
#include <string>

#include "ServerSocket.h"
#include "SocketException.h"

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

int main(int argc, char* argv[]) {
  try {
    // Create the Socket
    ServerSocket server(30000);

    while(true) {
      ServerSocket new_sock;
      server.accept(new_sock);

      try {
        while(true) {
          std::string data;
          new_sock >> data;
          std::cout << "[Received]\t" << data << std::endl;

          /* CODE BEGIN */
          new_sock << data;
          /* CODE  END  */
        }
      } catch(SocketException&) {}
    }
  } catch(SocketException& e) {
    std::cout<< "Exception caught: " << e.description() << std::endl;
  }

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


  timer.start();
  cout << "o_totalprice > 56789" << endl;
  auto res1 = Op::where(
      columnTable1.convertToInterResult(), 
      "o_totalprice", 
      Op::GT, 
    56789);
  cout <<  res1 -> getRowCount() << " rows are found." << endl;
  cout << timer.end() << "s elpased." << endl << endl;

  timer.start();
  cout << "5678 < o_totalprice < 56789" << endl; 
  auto res2 = Op::where(
      Op::where(
        columnTable1.convertToInterResult(), 
        "o_totalprice", 
        Op::LT, 
        56789
      ), 
      "o_totalprice", 
      Op::GT, 
      5678);
  
  cout << res2 -> getRowCount() << " rows are found." << endl;
  cout << timer.end() << "s elapsed." << endl << endl;


  timer.start();
  cout << "orders join lineitem" << endl;
  auto res3 = Op::join(
      columnTable1.convertToInterResult(), 
      columnTable2.convertToInterResult(), 
      "o_orderkey", 
      "l_orderkey");

  cout << res3 -> getRowCount() << " rows are found." << endl;
  res3 -> show();
  cout << timer.end() << "s elapsed." << endl << endl;

  timer.start();
  cout << "orders join lineitem where o_totalprice < 56789 and l_quantity > 40" << endl;
  auto res4 = Op::where(
      Op::where(
        Op::join(
          columnTable1.convertToInterResult(), 
          columnTable2.convertToInterResult(), 
        "o_orderkey", 
        "l_orderkey"),
        "o_totalprice",
        Op::LT,
        56789
      ),
      "l_quantity",
      Op::GT,
      40
    );
  cout << res4 -> getRowCount() << " rows are found." << endl;
  res4 -> show();
  cout << timer.end() << "s elapsed." << endl << endl;


  timer.start();
  cout << "orders join linitem where o_comment contains 'gift'" << endl;
  auto res5 = Op::join(
      Op::contains(
        columnTable1.convertToInterResult(),
        "o_comment",
        "gift",
        true
      ), 
      columnTable2.convertToInterResult(), 
      "o_orderkey", 
      "l_orderkey"
  );
  cout << res5 -> getRowCount() << " rows are found." << endl;
  res5 -> show();
  cout << timer.end() << "s elapsed." << endl << endl;

  return 0;
}
