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

struct SImage{
    uint8_t * imageData_;
    int width_;
    int height_;
    int bpp_;
};


class CImageOperator{
    SImage originalImage_;
    SImage workImage_;
    inline static int fromNormCoord(float normCoord, int size) { return round(normCoord * (float) size) + 0.001; }
    inline static float toNormCoord(int coord, int size) { return (float) coord / (float) size; }
    //equalise working image histogram
    void equaliseHistogram();
public:
    CImageOperator(const string& filepath);
    
    void init(const SProcessingInfo & processingInfo, uint_fast32_t newScaleToWidth, uint_fast32_t newScaleToHeight);
    void onResize(const SProcessingInfo &  processingInfo, uint_fast32_t newScaleToWidth, uint_fast32_t newScaleToHeight);
    ~CImageOperator();
    
    vector<vector<char>> drawWindow(const SProcessingInfo& processingInfo) const;
    vector<vector<char>> getAsciiImage(const SProcessingInfo& processingInfo, int width, int height);
    
    int getOriginalWidth() const { return originalImage_.width_; }
    int getOriginalHeight() const { return originalImage_.height_; }
};  
