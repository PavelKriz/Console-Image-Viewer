#include "CConsoleOperator.hpp"

bool CConsoleOperator::cursesInitialised_ = false;

CConsoleOperator::CConsoleOperator(){
    //init curses
    if(!cursesInitialised_){
        initscr();
        noecho();
        clear();
        cursesInitialised_ = true;
    }
}

CConsoleOperator::~CConsoleOperator(){
    if(cursesInitialised_){
        endwin();
        cursesInitialised_ = false;
    }
}

void CConsoleOperator::drawImage(const vector<vector<char>>& toDraw){
    for(size_t i = 0; i < toDraw.size(); ++i){
        for(size_t j = 0; j < toDraw[i].size(); ++j){
            mvaddch(i, j, toDraw[i][j]);
        }
    }
}

pair<uint_fast32_t, uint_fast32_t> CConsoleOperator::getConsoleSize(){
    uint_fast32_t cols, rows;
    getmaxyx(stdscr, rows, cols);
    return make_pair(cols, rows);
}