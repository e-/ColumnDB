
#include "ClientSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

using namespace std;

short arr[1000000];

int unique(int id){
  return id * 2 + 1;
}

int main(int argc, char* argv[]) {
    srand(0); //time(NULL));
    try {
      // Create the Socket
      ClientSocket c_socket("localhost", 30001);
      string reply;
      int id = 0;
      ofstream out("client.log");

      while(id < 1000000) {
        int r = rand() % 100;

        if(r < 30 || id == 0) {
          // insert
          string message = "INSERT|" + to_string(unique(id)) + "|0|0|asdfasdf";
          id++;
          out << "[Sending]\t" + message + "\n";
          cout << "[Sending]\t" + message + "\n";
          c_socket << message;
          c_socket >> reply;

          out << "[Response]\t" << reply << "\n";
          cout << "[Response]\t" << reply << "\n";
        }
        else if(r < 80) { 
          // update

          int t = rand() % id;
          arr[t] = rand() % 10000;
          string message = "UPDATE|" + to_string(unique(t)) + "|" + to_string(arr[t]) + "|" + to_string(arr[t]) + "|asdfasdf";
          out << "[Sending]\t" + message + "\n";
          cout << "[Sending]\t" + message + "\n";
          c_socket << message;
          c_socket >> reply;

          out << "[Response]\t" << reply << "\n";
          cout << "[Response]\t" << reply << "\n";
        }
        else{
          // scan
          int t = rand() % id;
          string message = "SCAN|" + to_string(unique(t));
          out << "[Sending]\t" + message + "\n";
          cout << "[Sending]\t" + message + "\n";
          c_socket << message;
          c_socket >> reply;

          out << "[Response]\t" << reply << "\n";
          cout << "[Response]\t" << reply << "\n";
          
          if(reply != to_string(unique(t)) + "|" + to_string(arr[t]) + "|" + to_string(arr[t]) + "|asdfasdf") {
            out << "[Inconsistency]\t" << reply << "\n";
            cout << "[Inconsistency]\t" << reply << "\n";
            return 0;
          }
        }
        this_thread::sleep_for(chrono::milliseconds(1));
      }
    } catch(SocketException& e) {
        std::cout << "Exception caught: " << e.description() << std::endl;
    }

    return 0;
}
