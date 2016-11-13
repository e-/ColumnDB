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

void split(const string &s, char delim, vector<string> &elems) {
  stringstream ss;
  ss.str(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

vector<string> split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, elems);
  return elems;
}

int main(int argc, char* argv[]) {
  ColumnTable columnTable("Test Database");
  
  columnTable.addColumn(new UnpackedColumn<int>("o_orderkey", intParser));
  columnTable.addColumn(new PackedColumn<string>("o_orderstatus", stringParser));
  columnTable.addColumn(new PackedColumn<int>("o_totalprice", intParser));
  columnTable.addColumn(new TextColumn("o_comment"));


  try {
    // Create the Socket
    ServerSocket server(30001);

    while(true) {
      ServerSocket new_sock;
      server.accept(new_sock);

      try {
        while(true) {
          string data;
          new_sock >> data;
          cout << "[Received]\t" << data << std::endl;

          // INSERT|5020067|"F"|215675|"e the grouches wake furiously about the furiously regular shea"
         
          vector<string> request = split(data, '|');
          if(!request.size()) continue;

          string command = request[0];
          vector<string> args(request.begin() + 1, request.end());

          if(command == "INSERT") {
            bool result = columnTable.insert(args);
            new_sock << "insert";
            if(result) 
              new_sock << "success";
          }
          else if(command == "UPDATE") {
            
          }
          else if(command == "SCAN") {

          }
        }
      } catch(SocketException&) {}
    }
  } catch(SocketException& e) {
    std::cout<< "Exception caught: " << e.description() << std::endl;
  }
  
  return 0;
}
