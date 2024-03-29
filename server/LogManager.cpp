#include "LogManager.h"

LogManager::LogManager(const string &path, bool recovery) {
  this -> path = path;
  file.open(path, (recovery ? ofstream::app : ofstream::out)); 
  th = make_shared<thread>(thread([=]{ log(); })); 
}

LogManager::~LogManager() {
  th -> join();
  file.close();
}

void LogManager::log() {
  while(1) {
    lock.lock();

    if(buffer.size()> 0) {
      for(auto &line : buffer){
        file << line << endl;
      }
      buffer.clear();
    }
    lock.unlock();
  }
}

void LogManager::append(const string &log) {
  lock.lock();
  buffer.push_back(log);
  lock.unlock();
}
