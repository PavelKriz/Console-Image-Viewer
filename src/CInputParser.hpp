#pragma once

#include <stdexcept>
#include <string>

#include "SParsedInput.hpp"

using namespace std;

class CInputParser{
    static const string quickHelp_;
    static const string help_;
public:
    static SParsedInput parseInput(int argc, const char *argv[]);
};
