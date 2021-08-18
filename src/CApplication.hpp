#pragma once

#include <ncurses.h> 
#include <string>
#include <memory>
#include <stdexcept>

#include "CLoopTimeManager.hpp"
#include "CConsoleOperator.hpp"
#include "CInputHandler.hpp"
#include "CInputParser.hpp"
#include "SParsedInput.hpp"
#include "CImageOperator.hpp"

class CApplication{
    CConsoleOperator * consoleOperator_ = nullptr;
    SParsedInput parsedInput_;
    unique_ptr<CImageOperator> imageOperator_;
public:
    //throw exceptions
    CApplication(int argc, const char *argv[]);
    ~CApplication();
    //throw exceptions
    int run();
};
