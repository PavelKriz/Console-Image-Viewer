#include "CImage.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"


CImage::CImage(const string& filepath){
    imageData_ = stbi_load(filepath.c_str(), &width_, &height_, &bpp_, 1);
    //the bpp is always the info about bpp of the original image but the stb will force and make it 3 or any number, unless it is set to 0 (last parameter of the load function)
    if(bpp_ != 1){
        bpp_ = 1;
    }
    if(imageData_ == NULL){
        throw invalid_argument("Image wasn't loaded!\nThe filepath of the image might not be correct.\n");
    }
}

CImage::CImage(const CImage & toCopy, int scaleToWidth, int scaleToHeight) 
    :
    imageData_(nullptr),
    width_(scaleToWidth),
    height_(scaleToHeight),
    bpp_(toCopy.bpp_)
{
    uint8_t * newImageData = new uint8_t [scaleToWidth * scaleToHeight * bpp_];
    stbir_resize_uint8(toCopy.imageData_, toCopy.width_, toCopy.height_, 0,
                       newImageData, scaleToWidth, scaleToHeight, 0, bpp_);

    imageData_ = newImageData;
}

CImage::~CImage(){
    delete [] imageData_;
}

void CImage::scale(int scaleToWidth, int scaleToHeight){
    uint8_t * newImageData = new uint8_t [scaleToWidth * scaleToHeight   * bpp_];
    for(int i = 0; i < scaleToHeight; ++i){
        for(int j = 0; j < scaleToWidth; ++j){
            int pixelX = fromNormCoord( toNormCoord(j, scaleToWidth), width_ );
            int pixelY = fromNormCoord( toNormCoord(i, scaleToHeight), height_ );
            for(int k = 0; k < bpp_; ++k){
                newImageData[(i * scaleToWidth + j ) * bpp_ + k] = imageData_[(pixelY * width_ + pixelX) * bpp_ + k];
            }            
        }
    }
    delete [] imageData_;
    imageData_ = newImageData;
    width_ = scaleToWidth;
    height_ = scaleToHeight;
}

void CImage::resizeToConsole(){
    int newWidth = width_ * 2;
    uint8_t * newImageData = new uint8_t [newWidth * height_ * bpp_];
    for(int i = 0; i < height_; ++i){
        for(int j = 0; j < width_; ++j){
            for(int k = 0; k < bpp_; ++k){
                newImageData[(i * newWidth + j * 2) * bpp_ + k] = imageData_[(i * width_ + j) * bpp_ + k];
                newImageData[(i * newWidth + j * 2 + 1) * bpp_ + k] = imageData_[(i * width_ + j) * bpp_ + k];  
            }
        }
    }
    delete imageData_; 
    imageData_ = newImageData;      
    width_ = newWidth;
}

void CImage::drawWindow(const SProcessingInfo& processingInfo) const {
    vector<char> palette;
    size_t paletteMaxIndex = 0;
    
    const char broadPalette[] = {' ','.' ,'\'' ,'`' ,'^' ,'\"' ,',' ,':' ,';' ,'I' ,'l' ,'!' ,'i' ,'>' ,'<' ,'~' ,'+' ,'_' ,'-' ,'?' ,']' ,'[' ,'}' ,'{' ,'1' ,')' ,'(' ,'|' ,'\\' ,'/' ,'t' ,'f' ,'j' ,'r' ,'x' ,'n' ,'u' ,'v' ,'c' ,'z' ,'X' ,'Y' ,'U' ,'J' ,'C' ,'L' ,'Q' ,'0' ,'O' ,'Z' ,'m' ,'w' ,'q' ,'p' ,'d' ,'b' ,'k' ,'h' ,'a' ,'o' ,'*' ,'#' ,'M' ,'W' ,'&' ,'8' ,'%' ,'B' ,'@' ,'$' };
    
    int broadPaletteMaxIndex = 69;
    const char simplePalete[] =  { ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};
    int simplePaletteMaxIndex = 9;
    if(processingInfo.grayscale_ == SProcessingInfo::EGrayscale::BROAD){
        for(size_t i = 0; i <= broadPaletteMaxIndex; ++i){
            palette.push_back(broadPalette[i]);
        }
        paletteMaxIndex = broadPaletteMaxIndex;
    } else {
        for(size_t i = 0; i <= simplePaletteMaxIndex; ++i){
            palette.push_back(simplePalete[i]);
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
    
    
    for(int i = 0; i < height_; ++i){
        for(int j = 0; j < width_; ++j){
            float normVal = (float) imageData_[(i * width_ + j) * bpp_ + 0] / 255.0;
            int paletteIndex = (int) (round(normVal * (float) (paletteMaxIndex)) + 0.01);
            mvaddch(i, j, palette[paletteIndex]);
        }
    }
}


