#include "CImageOperator.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

CImageOperator::CImageOperator(const string& filepath){
    //load originalimage
    originalImage_.imageData_ = stbi_load(filepath.c_str(), &originalImage_.width_, &originalImage_.height_, &originalImage_.bpp_, 1);
    //the bpp is always the info about bpp of the original image but the stb will force and make it 3 or any number, unless it is set to 0 (last parameter of the load function)
    if(originalImage_.bpp_ != 1){
        originalImage_.bpp_ = 1;
    }
    if(originalImage_.imageData_ == NULL){
        throw invalid_argument("Image wasn't loaded!\nThe filepath of the image might not be correct.\n");
    }
}

void CImageOperator::init(const SProcessingInfo & processingInfo, uint_fast32_t width, uint_fast32_t height){
    onResize(processingInfo, width, height);
}

void CImageOperator::onResize(const SProcessingInfo & processingInfo, uint_fast32_t newWidth, uint_fast32_t newHeight) 
{
    if(workImage_.imageData_ != nullptr){
        delete workImage_.imageData_;
    }
    //the allocation is needed the resize itself is not allocating memory
    workImage_.imageData_ = new uint8_t [newWidth * newHeight * originalImage_.bpp_];
    stbir_resize_uint8(originalImage_.imageData_, originalImage_.width_, originalImage_.height_, 0,
                       workImage_.imageData_, newWidth, newHeight, 0, originalImage_.bpp_);
    workImage_.height_ = newHeight;
    workImage_.width_ = newWidth;
    workImage_.bpp_ = originalImage_.bpp_;
    //equalise working image histogram
    if(processingInfo.histogramEqualisation_){
        equaliseHistogram();
    }
}

CImageOperator::~CImageOperator(){
    if(workImage_.imageData_ != nullptr){
        delete workImage_.imageData_;
    }
    if(originalImage_.imageData_ != nullptr){
        delete originalImage_.imageData_;
    }
}


void CImageOperator::equaliseHistogram(){
    if(workImage_.bpp_ != 1){
        throw invalid_argument("BPP has to be 1");
    }
    //value, quantity
    map<uint8_t, int> histogram;
    for(int i = 0; i < 256; ++i){
        histogram.insert(make_pair(i, 0));
    }

    for(int i = 0; i < workImage_.width_ * workImage_.height_ * workImage_.bpp_; ++i){
        ++histogram.at(workImage_.imageData_[i]);
    }

    int total = workImage_.width_ * workImage_.height_ * workImage_.bpp_;
    vector<double> CDF(256, 0.0);
    double sum = 0;
    int minIndex = 0;
    bool minNotFound = true;
    for(int i = 0; i < 256; ++i){
        sum += (double) histogram.at(i) / (double) total;
        if(minNotFound){
            if(abs(sum) - abs((double) 0.0) >= numeric_limits<double>::epsilon()){
                minIndex = i;
                minNotFound = false;
            }
        }
        CDF[i] = sum;
    }

    uint8_t * equlisedImageData = new uint8_t[workImage_.width_ * workImage_.height_ * workImage_.bpp_];
    for(int i = 0; i < workImage_.width_ * workImage_.height_ * workImage_.bpp_; ++i){
        equlisedImageData[i] = ((CDF[workImage_.imageData_[i]] - CDF[minIndex])/ ( (double) 1.0 - CDF[minIndex])) * 255.0 + 0.00001;
    }

    delete [] workImage_.imageData_;
    workImage_.imageData_ = equlisedImageData;
}

vector<vector<char>> CImageOperator::drawWindow(const SProcessingInfo& processingInfo) const {
    vector<char> palette;
    size_t paletteMaxIndex = 0;
    
    const char broadPalette[] = {' ','.' ,'\'' ,'`' ,'^' ,'\"' ,',' ,':' ,';' ,'I' ,'l' ,'!' ,'i' ,'>' ,'<' ,'~' ,'+' ,'_' ,'-' ,'?' ,']' ,'[' ,'}' ,'{' ,'1' ,')' ,'(' ,'|' ,'\\' ,'/' ,'t' ,'f' ,'j' ,'r' ,'x' ,'n' ,'u' ,'v' ,'c' ,'z' ,'X' ,'Y' ,'U' ,'J' ,'C' ,'L' ,'Q' ,'0' ,'O' ,'Z' ,'m' ,'w' ,'q' ,'p' ,'d' ,'b' ,'k' ,'h' ,'a' ,'o' ,'*' ,'#' ,'M' ,'W' ,'&' ,'8' ,'%' ,'B' ,'@' ,'$' };
    
    size_t broadPaletteMaxIndex = 69;
    const char simplePalette[] =  { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    size_t simplePaletteMaxIndex = 9;
    if(processingInfo.grayscale_ == SProcessingInfo::EGrayscale::BROAD){
        for(size_t i = 0; i <= broadPaletteMaxIndex; ++i){
            palette.push_back(broadPalette[i]);
        }
        paletteMaxIndex = broadPaletteMaxIndex;
    } else {
        for(size_t i = 0; i <= simplePaletteMaxIndex; ++i){
            palette.push_back(simplePalette[i]);
        }
        paletteMaxIndex = simplePaletteMaxIndex;
    }
    
    vector<vector<char>> retVect;
    for(int i = 0; i < workImage_.height_; ++i){
        retVect.push_back(vector<char>(workImage_.width_, ' '));
    }

    for(int i = 0; i < workImage_.height_; ++i){
        for(int j = 0; j < workImage_.width_; ++j){
            float normVal = (float) workImage_.imageData_[(i * workImage_.width_ + j) * workImage_.bpp_ + 0] / 255.0;
            int paletteIndex;
            if(processingInfo.isInversed_){
                paletteIndex = (int) (round( (1.0 - normVal) * (float) (paletteMaxIndex)) + 0.01);
            } else {                
                paletteIndex = (int) (round(normVal * (float) (paletteMaxIndex)) + 0.01);
            }
            retVect[i][j] = palette[paletteIndex];
        }
    }
    return retVect;
}

/*
vector<vector<char>> CImageOperator::getAsciiImage(const SProcessingInfo& processingInfo, int width, int height){
    uint8_t * scaledImageData = new uint8_t [width * height * bpp_];
    stbir_resize_uint8(imageData_, width_, height_, 0,
                       scaledImageData, width, height, 0, bpp_);


    vector<char> palette;
    size_t paletteMaxIndex = 0;
    
    const char broadPalette[] = {' ','.' ,'\'' ,'`' ,'^' ,'\"' ,',' ,':' ,';' ,'I' ,'l' ,'!' ,'i' ,'>' ,'<' ,'~' ,'+' ,'_' ,'-' ,'?' ,']' ,'[' ,'}' ,'{' ,'1' ,')' ,'(' ,'|' ,'\\' ,'/' ,'t' ,'f' ,'j' ,'r' ,'x' ,'n' ,'u' ,'v' ,'c' ,'z' ,'X' ,'Y' ,'U' ,'J' ,'C' ,'L' ,'Q' ,'0' ,'O' ,'Z' ,'m' ,'w' ,'q' ,'p' ,'d' ,'b' ,'k' ,'h' ,'a' ,'o' ,'*' ,'#' ,'M' ,'W' ,'&' ,'8' ,'%' ,'B' ,'@' ,'$' };
    
    size_t broadPaletteMaxIndex = 69;
    const char simplePalette[] =  { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    size_t simplePaletteMaxIndex = 9;
    if(processingInfo.grayscale_ == SProcessingInfo::EGrayscale::BROAD){
        for(size_t i = 0; i <= broadPaletteMaxIndex; ++i){
            palette.push_back(broadPalette[i]);
        }
        paletteMaxIndex = broadPaletteMaxIndex;
    } else {
        for(size_t i = 0; i <= simplePaletteMaxIndex; ++i){
            palette.push_back(simplePalette[i]);
        }
        paletteMaxIndex = simplePaletteMaxIndex;
    }
    
    vector<vector<char>> retVect;
    for(int i = 0; i < height; ++i){
        retVect.push_back(vector<char>(width, ' '));
    }

    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            float normVal = (float) scaledImageData[(i * width_ + j) * bpp_ + 0] / 255.0;
            int paletteIndex = (int) (round(normVal * (float) (paletteMaxIndex)) + 0.01);
            retVect[i][j] = palette[paletteIndex];
        }
    }

    return retVect;
}
*/


