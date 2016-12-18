#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <thread>

#include "ClientSocket.h" 
#include "SocketException.h" 

using namespace std;

static inline string &ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),
            not1(ptr_fun<int, int>(isspace))));
    return s;
}

// trim from end
static inline string &rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(),
            not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline string &trim(string &s) {
    return ltrim(rtrim(s));
}

map<int, string> m;

void handleText(const string& path) {

  ifstream ifs(path);
  string line;
  string token;
 
  while(getline(ifs, line)) {
    istringstream iss(line);
    line = trim(line);

    if(line.size() == 0) continue;

    getline(iss, token, ',');
    int num = stoi(token);

    m[num] = line;
  }
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

int main(){
  handleText("../client1/client.log");
  handleText("../client2/client.log");

  try {
    // Create the Socket
    ClientSocket c_socket[2] = {
      ClientSocket("localhost", 30001),
      ClientSocket("localhost", 30001)
    };

    string reply;
    int id = 0, inter = 0;

    for(auto &kv : m) {
      vector<string> p = split(kv.second, ',');
      c_socket[inter] << "SCAN|" + p[0];
      string reply;
      c_socket[inter] >> reply;
     
      for(auto &c : reply) {
        if(c == '|') c = ',';
      } 

      if(reply != kv.second) {
        cout << "Inconsistent" << reply << ' ' << kv.second << endl;
        return 1;
      }
      inter = !inter;
      this_thread::sleep_for(chrono::milliseconds(1));
    }

    cout << "Check Completed" << endl;
    
  } catch(SocketException& e) {
      std::cout << "Exception caught: " << e.description() << std::endl;
  }

 

  return 0;
}


