#pragma once

#include <string>

using namespace std;

struct SFileOutput{
    bool is_;
    int width_;
    int height_;
    string relativeFilepath_;
};

struct SParsedInput{
    enum EGrayscale{
    	BROAD,
    	SIMPLE
    };
    
    string relativeFilepathToImage_;
    EGrayscale grayscale_;
    bool histogramEqualisation_;
    bool inversed_;
    SFileOutput fileOutput_;
};
