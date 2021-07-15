#pragma once

#include "CInputParser"

class CApllication{
    SParsedInput parsedInput_;
    initCurses();
    exitCurses();
    //TODO
    //checkInput();
public:
    //throw exceotions
    CApplication(int argc, const char *argv[]);
    //throw exceotions
    int run();
}
