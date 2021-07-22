#pragma once

#include <ncurses.h> 
#include <string>
#include <math.h>
#include <stdexcept>
#include <vector>

#include "SProcessingInfo.hpp"


using namespace std;

class CImage{
    uint8_t * imageData_;
    int width_;
    int height_;
    int bpp_;
public:
    inline static int fromNormCoord(float normCoord, int size) { return round(normCoord * (float) size) + 0.001; }
    inline static float toNormCoord(int coord, int size) { return (float) coord / (float) size; }
    
    CImage(const string& filepath);
    CImage(const CImage & toCopy, int scaleToWidth, int scaleToHeight);
    ~CImage();
    
    int getWidth() const { return width_; }
    int getHeight() const { return height_    ; }
    void scale(int scaleToWidth, int scaleToHeight);
    void resizeToConsole();
    void drawWindow(const SProcessingInfo& processingInfo) const;
};  
