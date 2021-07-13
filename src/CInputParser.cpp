#include <stdexcept>
#include <string>

#include "CInputParser.hpp"
#include "SParsedInput.hpp"

using namespace std;

const string CInputParser::basicUsage_ = "Usage is following: CIViewer [relative path to the image to be displayed]";

SParsedInput CInputParser::parseInput(int argc, const char *argv[]){
    if(argc != 2){
        throw invalid_argument("Invalid number of arguments! " + basicUsage_);
    }
    
    SParsedInput parsedInput = {string(argv[1])};
    return parsedInput;
}
