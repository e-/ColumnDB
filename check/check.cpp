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

int main(){
  handleText("../client1/client.log");
  handleText("../client2/client.log");

  for(auto &kv : m) {
    cout << kv.second << endl;
  }

  return 0;
}


