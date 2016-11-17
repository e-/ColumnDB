#include <iostream>
#include <functional>
#include <fstream>
#include <string>
#include <chrono>
#include <mutex>
#include <thread>

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

string intToString(int a) {
  return to_string(a);
}

string stringParser(const string &s) {
  return string(s);
}

string stringToString(string s) {
  return s;
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

void collect(ColumnTable &columnTable) {
  while(1) {
    this_thread::sleep_for(chrono::milliseconds(2000));
    columnTable.lock.lock();
    columnTable.collect();
    columnTable.lock.unlock();
  }
}

int main(int argc, char* argv[]) {
  ColumnTable columnTable("Test Database");
  
  columnTable.addColumn(new UnpackedColumn<int>("o_orderkey", intParser, intToString));
  columnTable.addColumn(new PackedColumn<string>("o_orderstatus", stringParser, stringToString));
  columnTable.addColumn(new PackedColumn<int>("o_totalprice", intParser, intToString));
  columnTable.addColumn(new UnpackedColumn<string>("o_comment", stringParser, stringToString));


  try {
    // Create the Socket
    ServerSocket server(30001);
    
    thread collector(collect, ref(columnTable));

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

          columnTable.lock.lock();
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
          columnTable.lock.unlock();
        }
      } catch(SocketException&) {}
    }
    
    collector.join();

  } catch(SocketException& e) {
    std::cout<< "Exception caught: " << e.description() << std::endl;
  }
  
  return 0;
}
