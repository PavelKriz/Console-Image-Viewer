#include <stdexcept>
#include <string>
#include <cstring>
#include <vector>

#include "CInputParser.hpp"
#include "SParsedInput.hpp"

using namespace std;

const string CInputParser::basicUsage_ = "Basic usage is following: CIViewer [relative path to the image to be displayed]";

const string CInputParser::help_ = "Usage is following:\n"
                                   "basic usage: CIViewer [relative path to the image to be displayed] [arguments]\n"
                                   "\n"
                                   "Possible arguments are following:\n"
                                   "-h          for help\n"
                                   "-s [value]  for choosing the display scale\n"
                                   "    following argument has to be:\n"
                                   "    [simple] or [broad] (default)\n"
                                   "    eg: -s simple\n"
                                   "-dhe    disables histogram equalisation\n";
                                   
SParsedInput CInputParser::parseInput(int argc, const char *argv[]){
    
    SParsedInput parsedInput;
    //default values
    parsedInput.grayscale_ = SParsedInput::EGrayscale::BROAD;
    parsedInput.histogramEqualisation_ = true;
    
    vector<bool> used(argc, false);
    used[0] = true;
    
    for(int i = 0; i < argc; ++i){
    	if(strcmp(argv[i], "-s") == 0){
   	        used[i] = true;
   	        ++i;
   	        if(i >= argc){
   	            throw invalid_argument("Invalid arguments! Try help: \"CIViewer -h\" or:\n" + basicUsage_);
   	        }
   	        used[i] = true;
   	        
   	        if(strcmp(argv[i], "broad") == 0){
   	            parsedInput.grayscale_ = SParsedInput::EGrayscale::BROAD;
   	        } else if (strcmp(argv[i], "simple") == 0){
   	            parsedInput.grayscale_ = SParsedInput::EGrayscale::SIMPLE;
   	        } else {
   	            throw invalid_argument("Invalid arguments! Try help: \"CIViewer -h\" or:\n" + basicUsage_);
   	        }
    	}
    	if(strcmp(argv[i], "-h") == 0){
    	    used[i] = true;
   	        throw invalid_argument(help_);
    	}
        if(strcmp(argv[i], "-dhe") == 0){
            used[i] = true;
            parsedInput.histogramEqualisation_ = false;
        }
    }
    
    for(size_t i = 0; i < argc; ++i){
        if(!used[i]){
            parsedInput.relativeFilepathToImage_ = string(argv[i]);
            break;
        }
    }
    
    return parsedInput;
}
