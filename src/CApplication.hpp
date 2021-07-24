#pragma once

#include <ncurses.h> 
#include <string>
#include <memory>
#include <stdexcept>

#include "CLoopTimeManager.hpp"
#include "CInputParser.hpp"
#include "SParsedInput.hpp"
#include "CImage.hpp"

class CApplication{
    static bool cursesInitialised_; //false in the beggining
    SParsedInput parsedInput_;
    unique_ptr<CImage> image_;
    static void initCurses();
    static void exitCurses();
public:
    //throw exceptions
    CApplication(int argc, const char *argv[]);
    //throw exceptions
    int run();
};
