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
    SProcessingInfo processingInfo;
        if(parsedInput_.grayscale_ == SParsedInput::EGrayscale::BROAD){
            processingInfo.grayscale_ = SProcessingInfo::EGrayscale::BROAD;
        } else {
            processingInfo.grayscale_ = SProcessingInfo::EGrayscale::SIMPLE;
        }
    processingInfo.histogramEqualisation_ = parsedInput_.histogramEqualisation_;
    processingInfo.isInversed_ = false;
    if(parsedInput_.fileOutput_.is_){
        if(parsedInput_.inversed_){
            processingInfo.isInversed_ = false;
        } else {
            processingInfo.isInversed_ = true;
        }
    } else {
        processingInfo.isInversed_ = parsedInput_.inversed_;
    }

    //there is only file output
    if(parsedInput_.fileOutput_.is_){
        CFileSaver fileSaver(parsedInput_.fileOutput_.relativeFilepath_);
        float ratio = (float) imageOperator_->getOriginalHeight() / (float) imageOperator_->getOriginalWidth();
        imageOperator_->onResize( processingInfo, parsedInput_.fileOutput_.width_, (int) parsedInput_.fileOutput_.width_ * ratio * 0.5);
        auto retVect = imageOperator_->drawWindow(processingInfo);
        try{
            fileSaver.safeFile(retVect);
        } catch (const ofstream::failure& f){
            printw(f.what());
            return 1;
        }
        return 0;
    }

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
        
        if(prevCols != cols || prevRows != rows){
            prevCols = cols;
            prevRows = rows;

            int width = imageOperator_->getOriginalWidth();
            int height = imageOperator_->getOriginalHeight();

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

            //prepare the image
            imageOperator_->onResize(processingInfo, newSizeX, newSizeY);        

            //draw the image
            //first clear
            clear();
            //then draw
            auto retCharImage = imageOperator_->drawWindow(processingInfo);
            
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

