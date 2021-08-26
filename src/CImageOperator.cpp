#include "CImageOperator.hpp"

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

void CImageOperator::init(uint_fast32_t width, uint_fast32_t height){
    onResize(width, height);
}

void CImageOperator::onResize(uint_fast32_t newWidth, uint_fast32_t newHeight) 
{
    workImage_.imageData_ = new uint8_t [newWidth * newHeight * originalImage_.bpp_];
    stbir_resize_uint8(originalImage_.imageData_, originalImage_.width_, originalImage_.height_, 0,
                       workImage_.imageData_, newWidth, newHeight, 0, originalImage_.bpp_);
    workImage_.height_ = newScaleToWidth;
    
}

CImageOperator::~CImageOperator(){
    delete imageData_;
}


void CImageOperator::equaliseHistogram(){
    if(equalised_){
        return;
    }
    if(bpp_ != 1){
        throw("BPP has to be 1");
    }
    //value, quantity
    map<uint8_t, int> histogram;
    for(int i = 0; i < 256; ++i){
        histogram.insert(make_pair(i, 0));
    }

    for(int i = 0; i < width_ * height_ * bpp_; ++i){
        ++histogram.at(imageData_[i]);
    }

    int total = width_ * height_ * bpp_;
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

    uint8_t * equlisedImageData = new uint8_t[width_ * height_ * bpp_];
    for(int i = 0; i < width_ * height_ * bpp_; ++i){
        equlisedImageData[i] = ((CDF[imageData_[i]] - CDF[minIndex])/ ( (double) 1.0 - CDF[minIndex])) * 255.0 + 0.00001;
    }

    delete [] imageData_;
    imageData_ = equlisedImageData;
    equalised_ = true;
}

void CImageOperator::scale(int scaleToWidth, int scaleToHeight){
    uint8_t * newImageData = new uint8_t [scaleToWidth * scaleToHeight   * bpp_];
    stbir_resize_uint8(imageData_, width_, height_, 0,
                       newImageData, scaleToWidth, scaleToHeight, 0, bpp_);
    delete [] imageData_;
    imageData_ = newImageData;
    width_ = scaleToWidth;
    height_ = scaleToHeight;
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
    
    /*
    for(int i = 0; i <= paletteMaxIndex; ++i){
        printw("%d %c |", i, palette[i]);
    }
    refresh();
    getch();
    */
    
    vector<vector<char>> retVect;
    for(int i = 0; i < height_; ++i){
        retVect.push_back(vector<char>(width_, ' '));
    }

    for(int i = 0; i < height_; ++i){
        for(int j = 0; j < width_; ++j){
            float normVal = (float) imageData_[(i * width_ + j) * bpp_ + 0] / 255.0;
            int paletteIndex = (int) (round(normVal * (float) (paletteMaxIndex)) + 0.01);
            retVect[i][j] = palette[paletteIndex];
        }
    }

    return retVect;
}

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
    
    /*
    for(int i = 0; i <= paletteMaxIndex; ++i){
        printw("%d %c |", i, palette[i]);
    }
    refresh();
    getch();
    */
    
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



