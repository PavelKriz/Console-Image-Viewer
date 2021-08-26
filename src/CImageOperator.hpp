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
    bool equalised_ = false;

    inline static int fromNormCoord(float normCoord, int size) { return round(normCoord * (float) size) + 0.001; }
    inline static float toNormCoord(int coord, int size) { return (float) coord / (float) size; }
public:
    CImageOperator(const string& filepath);
    
    void init(uint_fast32_t newScaleToWidth, uint_fast32_t newScaleToHeight);
    void onResize(uint_fast32_t newScaleToWidth, uint_fast32_t newScaleToHeight);
    ~CImageOperator();
    
    //equalise image if it wasn't equalised before
    void equaliseHistogram();
    void scale(int scaleToWidth, int scaleToHeight);
    vector<vector<char>> drawWindow(const SProcessingInfo& processingInfo) const;
    vector<vector<char>> getAsciiImage(const SProcessingInfo& processingInfo, int width, int height);
    
    int getOriginalWidth() const { return originalImage_.width_; }
    int getOriginalHeight() const { return originalImage_.height_; }
};  
