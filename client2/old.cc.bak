
#include "ClientSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char* argv[]) {
    try {
        // Create the Socket
        ClientSocket c_socket("localhost", 30001);
        std::string line;
        std::string reply;
        int id = 1;
        
        while(1) {
          std::ifstream in("orders_sample.tbl");
          std::ofstream out("client.log");

          while(!in.eof()) {
              getline(in, line);
              if(line != "") {
                  try {
                      std::string message = "INSERT|" + std::to_string(id++) + "|" + line;
                      char prev = ' ';
                      for(char &c : message) {
                        if(c == ',' && prev != '\\')
                          c = '|';
                        prev = c;
                      }
                      out << "[Sending]\t" + message + "\n";
                      std::cout << "[Sending]\t" + message + "\n";
                      c_socket << message;
                      c_socket >> reply;
                  } catch (SocketException&) {}
                  out << "[Response]\t" << reply << "\n";
                  std::cout << "[Response]\t" << reply << "\n";
              }
//              this_thread::sleep_for(chrono::milliseconds(30));

          }
        }
    } catch(SocketException& e) {
        std::cout << "Exception caught: " << e.description() << std::endl;
    }

    return 0;
}
