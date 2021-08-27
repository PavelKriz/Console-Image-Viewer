#include "CInputParser.hpp"

#include <iostream>

using namespace std;

const string CInputParser::quickHelp_ = "Try help: \"CIViewer -h\" or:\nBasic usage is following: CIViewer [relative path to the image to be displayed]";

const string CInputParser::help_ = "Usage is following:\n"
                                   "basic usage: CIViewer [relative path to the image to be displayed] [arguments]\n"
                                   "\n"
                                   "Possible arguments are following:\n"
                                   "-h          for help\n"
                                   "-s [value]  for choosing the display scale\n"
                                   "    following argument has to be:\n"
                                   "    [simple] or [broad] (default)\n"
                                   "    eg: -s simple\n"
                                   "-dhe    disables histogram equalisation\n"
                                   "-i  inverses the scale (behaves differently with file or console output)\n"
                                   "(Program can be quit by pressing q or Q)\n"
                                   "-f  for file output, usage is:\n"
                                   "    -f [width in symbols] [output file filepath]";
                                   
SParsedInput CInputParser::parseInput(int argc, const char *argv[]){
    
    SParsedInput parsedInput;
    //default values
    parsedInput.grayscale_ = SParsedInput::EGrayscale::SIMPLE;
    parsedInput.histogramEqualisation_ = true;
    parsedInput.fileOutput_.is_ = false;
    parsedInput.inversed_ = false;
    
    vector<bool> used(argc, false);
    used[0] = true;
    
    if(argc <= 1){
        throw invalid_argument("Too few arguments!\n" + quickHelp_);
    }

    for(int i = 0; i < argc; ++i){
    	if(strcmp(argv[i], "-s") == 0){
   	        used[i] = true;
   	        ++i;
   	        if(i >= argc){
   	            throw invalid_argument("Invalid arguments!\n" + quickHelp_);
   	        }
   	        
   	        if(strcmp(argv[i], "broad") == 0){
   	            parsedInput.grayscale_ = SParsedInput::EGrayscale::BROAD;
   	        } else if (strcmp(argv[i], "simple") == 0){
   	            parsedInput.grayscale_ = SParsedInput::EGrayscale::SIMPLE;
   	        } else {
   	            throw invalid_argument("Invalid arguments!" + quickHelp_);
   	        }
   	        used[i] = true;
    	}
    	else if(strcmp(argv[i], "-h") == 0){
    	    used[i] = true;
   	        throw invalid_argument(help_);
    	}
        else if(strcmp(argv[i], "-dhe") == 0){
            used[i] = true;
            parsedInput.histogramEqualisation_ = false;
        }
        else if(strcmp(argv[i], "-i") == 0){
            used[i] = true;
            parsedInput.inversed_ = true;
            cout << "parsed input inversed = true" << endl;
        }
        else if(strcmp(argv[i], "-f") == 0){
            parsedInput.fileOutput_.is_ = true;
            used[i] = true;
            ++i;

            if(i + 1 >= argc){
   	            throw invalid_argument("Invalid arguments!\n" + quickHelp_ + to_string(i));
   	        }
            

            parsedInput.fileOutput_.width_ = atoi(argv[i]);
            used[i] = true;
            ++i;
            parsedInput.fileOutput_.relativeFilepath_ = string(argv[i]);
            used[i] = true;
        }
        
    }
    
    for(int i = 0; i < argc; ++i){
        if(!used[i]){
            parsedInput.relativeFilepathToImage_ = string(argv[i]);
            break;
        }
    }
    
    return parsedInput;
}
