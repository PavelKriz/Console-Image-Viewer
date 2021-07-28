#pragma once

#include <vector>
#include <curses.h>

#include "ESignal.hpp"

using namespace std;

class CInputHandler{
    bool isEnd_;
public:
    CInputHandler() : isEnd_(false) {}
    inline bool end() {return isEnd_;}
    vector<ESignal> getInputSignals();
};