#pragma once

#include <string>

using namespace std;

struct SParsedInput{
    enum EGrayscale{
    	BROAD,
    	SIMPLE
    };
    
    string relativeFilepathToImage_;
    EGrayscale grayscale_;
};
