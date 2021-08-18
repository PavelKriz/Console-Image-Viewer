#pragma once

#include <vector>
#include <ncurses.h>
#include <utility>


using namespace std;

class CConsoleOperator{
    static bool cursesInitialised_; //false in the beggining

    public:
    CConsoleOperator();
    ~CConsoleOperator();
    void drawImage(const vector<vector<char>>& toDraw);
    /**
     * @brief Get the size of the console
     * 
     * @return pair<uint_fast32_t, uint_fast32_t> <#columns, #rows> 
     */
    static pair<uint_fast32_t, uint_fast32_t> getConsoleSize();
};