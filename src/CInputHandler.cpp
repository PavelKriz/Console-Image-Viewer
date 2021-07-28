#include "CInputHandler.hpp"

vector<ESignal> CInputHandler::getInputSignals(){
    char c = getch();
    if(c == 'q' || c == 'Q'){
        isEnd_ = true;
        return vector<ESignal>(ESignal::QUIT);
    }

    return vector<ESignal>();
}