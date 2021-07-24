#include "CLoopTimeManager.hpp"

CLoopTimeManager::CLoopTimeManager()
{
}

void CLoopTimeManager::loopBegin(){
    loopBeginTime_ = chrono::steady_clock::now();
}

void CLoopTimeManager::loopEnd(){
    auto loopEndTime_ = chrono::steady_clock::now();
    auto microSecondsDiff = chrono::duration_cast<chrono::microseconds>( loopEndTime_ - loopBeginTime_ ).count();
    timeDiff_ = microSecondsDiff;
}

float CLoopTimeManager::getSleepTime(){
    return sleep_;
}

void CLoopTimeManager::sleep(){
    int64_t desiredLoopTimeMicroSeconds = ( 1.0 / desiredFPS_) * 1000000.0;
    if(timeDiff_ < desiredLoopTimeMicroSeconds){
        usleep(desiredLoopTimeMicroSeconds - timeDiff_);
    } 
    //sleep_ = desiredLoopTimeMicroSeconds;
    sleep_ = desiredLoopTimeMicroSeconds - timeDiff_;
}