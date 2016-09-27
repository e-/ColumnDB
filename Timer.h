#ifndef TIMER_H
#define TIMER_H

#include <ctime>

using namespace std;

class Timer
{
public:
  Timer() {}
  void start() {clock_gettime(CLOCK_MONOTONIC, &begin);}
  double end() {
    clock_gettime(CLOCK_MONOTONIC, &finish);
    double elapsed = finish.tv_sec - begin.tv_sec;
    elapsed += (finish.tv_nsec - begin.tv_nsec) / 1000000000.0;
    return elapsed;
//    return double(clock() - begin) / CLOCKS_PER_SEC;
  }

protected:
  struct timespec begin, finish;
};

#endif

