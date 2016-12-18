#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <mutex>
#include <fstream>
#include <memory>
#include <thread>
#include <iostream>
#include <vector>

using namespace std;

class LogManager
{
public:
  LogManager(const string &path, bool append);
  ~LogManager();

  void start();
  void log();  
  void append(const string &log);

  mutex lock;

protected:
  shared_ptr<thread> th;
  string path;
  ofstream file;
  vector<string> buffer;
};

#endif

