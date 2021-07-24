#pragma once

#include <chrono>
#include <unistd.h>
#include <limits>

using namespace std;

class CLoopTimeManager{
    const float desiredFPS_ = 30.0;
    int64_t timeDiff_ = 0;
    int64_t sleep_ = 0;
    std::chrono::time_point<std::chrono::steady_clock> loopBeginTime_;
public:
    CLoopTimeManager();
    void loopBegin();
    void loopEnd();
    void sleep();
    float getSleepTime();
};