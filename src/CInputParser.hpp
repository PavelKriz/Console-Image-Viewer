#pragma once

#include <stdexcept>
#include <string>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>

#include "CInputParser.hpp"
#include "SParsedInput.hpp"

using namespace std;

#include "SParsedInput.hpp"

using namespace std;

class CInputParser{
    static const string quickHelp_;
    static const string help_;
public:
    static SParsedInput parseInput(int argc, const char *argv[]);
};
