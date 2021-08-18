#pragma once

#include <ncurses.h> 
#include <string>
#include <math.h>
#include <stdexcept>
#include <vector>
#include <map>
#include <limits>

#include "SProcessingInfo.hpp"


using namespace std;

class CImageOperator{
    uint8_t * imageData_;
    int width_;
    int height_;
    int bpp_;
    bool equalised_ = false;

    inline static int fromNormCoord(float normCoord, int size) { return round(normCoord * (float) size) + 0.001; }
    inline static float toNormCoord(int coord, int size) { return (float) coord / (float) size; }
    
    
    //not used anymore
    void resizeToConsole();
public:
    CImageOperator(const string& filepath);
    CImageOperator(const CImageOperator & toCopy, int scaleToWidth, int scaleToHeight);
    ~CImageOperator();
    
    //equalise image if it wasn't equalised before
    void equaliseHistogram();
    void scale(int scaleToWidth, int scaleToHeight);
    vector<vector<char>> drawWindow(const SProcessingInfo& processingInfo) const;
    vector<vector<char>> getAsciiImage(const SProcessingInfo& processingInfo, int width, int height);
    
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
};  
