#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>

class Timer {
  public:
    using TimePoint = std::chrono::high_resolution_clock::time_point;

    Timer() : start(std::chrono::high_resolution_clock::now()) {};

    TimePoint getCurTime() { return std::chrono::high_resolution_clock::now(); }

    double elapsed() {
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
    }

  private:
    std::chrono::high_resolution_clock::time_point start;
};

#endif // __TIMER_H__