#include "CApplication.hpp"

CApplication::CApplication(int argc, const char *argv[]){
    try{
        consoleOperator_ = new CConsoleOperator();
        parsedInput_ = CInputParser::parseInput(argc, argv);
        imageOperator_ = make_unique<CImageOperator>(parsedInput_.relativeFilepathToImage_);
    } catch (const invalid_argument& ia){
        printw(ia.what());
        printw("\nPress any key to end...\n");
        refresh();
        getch();
        if(consoleOperator_ != nullptr){
            delete consoleOperator_;
        }
        throw 1;
    }
}

CApplication::~CApplication(){
    if(consoleOperator_ != nullptr){
        delete consoleOperator_;
    }
}

int CApplication::run(){

    int prevCols = 0;
    int prevRows = 0;
    CLoopTimeManager loopTimeManager;
    CInputHandler inputHandler;
    CConsoleOperator consoleOperator;

    while(!inputHandler.end()){
        loopTimeManager.loopBegin();
        int cols, rows;
        getmaxyx(stdscr, rows, cols);
        pair<uint_fast32_t, uint_fast32_t> consoleSize = CConsoleOperator::getConsoleSize();
        cols = consoleSize.first;
        rows = consoleSize.second;
        int width = imageOperator_->getWidth();
        int height = imageOperator_->getHeight();
        
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
            CImageOperator currentImageOperator((*imageOperator_), newSizeX, newSizeY);
        
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
                currentImageOperator.equaliseHistogram();
            }
            //then draw
            auto retCharImage = currentImageOperator.drawWindow(processingInfo);
            
            consoleOperator.drawImage(retCharImage);
        }
        refresh();

        //get the input
        inputHandler.getInputSignals();

        loopTimeManager.loopEnd();
        //printw("sleep duration: %f", loopTimeManager.getSleepTime());
        loopTimeManager.sleep();
    }
    
    return 0;   
}

