#include "CApplication.hpp"

void CApplication::prepareProcessingInfo(){
        if(parsedInput_.grayscale_ == SParsedInput::EGrayscale::BROAD){
            processingInfo_.grayscale_ = SProcessingInfo::EGrayscale::BROAD;
        } else {
            processingInfo_.grayscale_ = SProcessingInfo::EGrayscale::SIMPLE;
        }
    processingInfo_.histogramEqualisation_ = parsedInput_.histogramEqualisation_;
    processingInfo_.isInversed_ = false;
    if(parsedInput_.fileOutput_.is_){
        if(parsedInput_.inversed_){
            processingInfo_.isInversed_ = false;
        } else {
            processingInfo_.isInversed_ = true;
        }
    } else {
        processingInfo_.isInversed_ = parsedInput_.inversed_;
    }

}

int CApplication::runFileOutput(){
    CFileSaver fileSaver(parsedInput_.fileOutput_.relativeFilepath_);
        float ratio = (float) imageOperator_->getOriginalHeight() / (float) imageOperator_->getOriginalWidth();
        imageOperator_->onResize( processingInfo_, parsedInput_.fileOutput_.width_, (int) parsedInput_.fileOutput_.width_ * ratio * 0.5);
        auto retVect = imageOperator_->drawWindow(processingInfo_);
        try{
            fileSaver.safeFile(retVect);
        } catch (const ofstream::failure& f){
            printw(f.what());
            return 1;
        }
        return 0;
}

int CApplication::runLiveView(){
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
            imageOperator_->onResize(processingInfo_, newSizeX, newSizeY);        

            //draw the image
            //first clear
            clear();
            //then draw
            auto retCharImage = imageOperator_->drawWindow(processingInfo_);
            
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


CApplication::CApplication(int argc, const char *argv[]){
    try{
        consoleOperator_ = new CConsoleOperator();
        parsedInput_ = CInputParser::parseInput(argc, argv);
        imageOperator_ = make_unique<CImageOperator>(parsedInput_.relativeFilepathToImage_);
        prepareProcessingInfo();
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
    if(parsedInput_.fileOutput_.is_){
        //there is only file output
        return runFileOutput();
    } else {
        return runLiveView();
    }  
}