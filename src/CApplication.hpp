#pragma once

#include <ncurses.h> 
#include <string>
#include <memory>
#include <stdexcept>

#include "CInputParser.hpp"
#include "SParsedInput.hpp"
#include "CImage.hpp"

class CApplication{
    SParsedInput parsedInput_;
    void initCurses();
    void exitCurses();
    //TODO
    //checkInput();
public:
    //throw exceotions
    CApplication();
    //throw exceotions
    int run(int argc, const char *argv[]);
};
