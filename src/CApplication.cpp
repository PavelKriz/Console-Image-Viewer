#include "CApplication.hpp"

bool CApplication::cursesInitialised_ = false;

void CApplication::initCurses(){
    if(!cursesInitialised_){
        cursesInitialised_ = true;
        initscr();
        noecho();
        clear();
    }
}

void CApplication::exitCurses(){
    if(cursesInitialised_){
        endwin();
    }
}

CApplication::CApplication(int argc, const char *argv[]){
    try{
        initCurses();
        parsedInput_ = CInputParser::parseInput(argc, argv);
        image_ = make_unique<CImage>(parsedInput_.relativeFilepathToImage_);
    } catch (const invalid_argument& ia){
        printw("Error occurred\n");
        printw(ia.what());
        printw("\nPress any key to end...\n");
        refresh();
        getch();
        exitCurses();
        throw 1;
    }
}

int CApplication::run(){

    int prevCols = 0;
    int prevRows = 0;
    CLoopTimeManager loopTimeManager;
    CInputHandler inputHandler;
    while(!inputHandler.end()){
        loopTimeManager.loopBegin();
        int cols, rows;
        getmaxyx(stdscr, rows, cols);
        
        int width = image_->getWidth();
        int height = image_->getHeight();
        
        float scaleX = ((float) cols / 2.0) / (float) width;
        float scaleY = (float) rows / (float) height;
        
        float scaleFactor = 0.0;
        if(scaleX > scaleY){
            //scale along x
            scaleFactor = scaleY;
        } else {
            //scale along y
            scaleFactor = scaleX;
        }
        
        int newSizeX = scaleFactor * width * 2;
        int newSizeY = scaleFactor * height;
        
        /*
        printw("y: %d x: %d", rows, cols);
        printw("newSizeY: %d newSizeX: %d", newSizeY, newSizeX);
        getch();
        */
        if(prevCols != cols || prevRows != rows){
            prevCols = cols;
            prevRows = rows;

            //prepare the image
            CImage currentImage((*image_), newSizeX, newSizeY);
        
            /*
            printw("scaleX: %f scaleY: %f, scale factor %f \n", scaleX, scaleY, scaleFactor);
            printw("cols: %d rows: %d \n", cols, rows);
            printw("newSizeY: %d newSizeX: %d \n", newSizeY, newSizeX);
            */
            SProcessingInfo processingInfo;
            if(parsedInput_.grayscale_ == SParsedInput::EGrayscale::BROAD){
                processingInfo.grayscale_ = SProcessingInfo::EGrayscale::BROAD;
            } else {
                processingInfo.grayscale_ = SProcessingInfo::EGrayscale::SIMPLE;
            }
            processingInfo.histogramEqualisation_ = parsedInput_.histogramEqualisation_;

            //draw the image
            //first clear
            clear();
            //equalise
            if(processingInfo.histogramEqualisation_){
                currentImage.equaliseHistogram();
            }
            //then draw
            currentImage.drawWindow(processingInfo);
        }
        refresh();

        //get the input
        inputHandler.getInputSignals();

        loopTimeManager.loopEnd();
        //printw("sleep duration: %f", loopTimeManager.getSleepTime());
        loopTimeManager.sleep();
    }
    
    exitCurses();
    return 0;   
}

