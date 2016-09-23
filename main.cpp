#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define CSV_IO_NO_THREAD
#include "lib/csv.h"

using namespace std;

//case class aOrderItem(*O_ORDERKEY: Int, O_CUSTKEY: Int, *O_ORDERSTATUS: String, *O_TOTALPRICE: Double, O_ORDERDATE: String, O_ORDERPRIORITY: String, O_CLERK: String, O_SHIPPRIORITY: Int, *O_COMMENT: String)


int main(int argc, char *argv[]) {
  if(argc == 1) { 
    cout << "Please specify the data file" << endl;
    return 1;
  }
  
  io::CSVReader<9, io::trim_chars<' '>, io::double_quote_escape<',', '\"'>> in(argv[1]);
  in.set_header("OrderKey", "CustKey", "OrderStatus", "TotalPrice", "OrderDate", "OrderPriority", "Clerk", "ShipPriority", "Comment");

  int orderKey, custKey, shipPriority;
  string orderStatus, orderDate, orderPriority, clerk, comment, totalPrice;

  int rowCount = 0;

  while(in.read_row(orderKey, custKey, orderStatus, totalPrice, orderDate, orderPriority, clerk, shipPriority, comment)) {
    rowCount++;
    //cout << orderKey << ' ' << orderStatus << ' ' << totalPrice << ' ' << comment << endl;
  }
  cout << rowCount << " rows have been processed." << endl;
  return 0;
}
