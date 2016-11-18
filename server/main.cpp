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

int i=0;
void collect(ColumnTable &columnTable) {
  int cnt = 0;
  while(1) {
    cnt++;
    this_thread::sleep_for(chrono::milliseconds(2000));
    cout << "======================================" << endl;
    cout << "total tx processed: " << i << endl;
    cout << "total rows inserted: " << columnTable.getRowCount() << endl;
    cout << "total garbage collected: " << cnt << endl;

    columnTable.lock.lock();
    Timer timer;
    timer.start();
    columnTable.collect();
    cout << timer.end() << " elapsed" << endl;
    columnTable.lock.unlock();
  }
}

int main(int argc, char* argv[]) {
  ColumnTable columnTable("Test Database");
  
  columnTable.addColumn(shared_ptr<Column>(new UnpackedColumn<int>("o_orderkey", intParser, intToString)));
  columnTable.addColumn(shared_ptr<Column>(new PackedColumn<string>("o_orderstatus", stringParser, stringToString)));
  columnTable.addColumn(shared_ptr<Column>(new PackedColumn<int>("o_totalprice", intParser, intToString)));
  columnTable.addColumn(shared_ptr<Column>(new UnpackedColumn<string>("o_comment", stringParser, stringToString)));


  try {
    // Create the Socket
    ServerSocket server(30001);
    thread collector(collect, ref(columnTable));

    while(true) {
      ServerSocket new_socks[2];
      server.accept(new_socks[0]);
      server.accept(new_socks[1]);
      int flag = 0;

      try {
        while(true) {
          i++;
          string data;
          ServerSocket &new_sock = new_socks[flag];
          flag = !flag;
          new_sock >> data;
//          cerr << data << '\n';
         
          vector<string> request = split(data, '|');
          if(!request.size()) continue;

          string command = request[0];
          vector<string> args(request.begin() + 1, request.end());

          columnTable.lock.lock();
          if(command == "INSERT") {
            bool result = columnTable.insert(args);
            if(result) 
              new_sock << "insert success";
          }
          else if(command == "UPDATE") {
            bool result = columnTable.update(args[0], args);
            if(result) 
              new_sock << "update success";
          }
          else if(command == "SCAN") {
            vector<string> result = columnTable.scan(args[0]);
            new_sock << result[0] + "|" + result[1] + "|" + result[2] + "|" + result[3];
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
