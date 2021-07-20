#include "CApplication.hpp"

bool CApplication::cursesInitialised_ = false;

void CApplication::initCurses(){
    cursesInitialised_ = true;
    initscr();
    noecho();
    clear();
}

void CApplication::exitCurses(){
    endwin();
}


CApplication::CApplication(int argc, const char *argv[]){
    initCurses();
    parsedInput_ = CInputParser::parseInput(argc, argv);
    image_ = make_unique<CImage>(parsedInput_.relativeFilepathToImage_);
}

int CApplication::run(){

    int prevCols = 0;
    int prevRows = 0;
    while(true){
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
            clear();
        }
        
        CImage currentImage((*image_), newSizeX, newSizeY);
        
        /*
        printw("scaleX: %f scaleY: %f, scale factor %f \n", scaleX, scaleY, scaleFactor);
        printw("cols: %d rows: %d \n", cols, rows);
        printw("newSizeY: %d newSizeX: %d \n", newSizeY, newSizeX);
        */
        currentImage.drawWindow();
        //printw("newSizeY: %d newSizeX: %d \n", newSizeY, newSizeX);
        refresh();
    }
    
    exitCurses();
    return 0;   
}


int CApplication::handleErrors(const exception &e){
    if(!cursesInitialised_){
        initCurses();
    }
    printw(e.what());
    printw("\nPress any key to end...\n");
    refresh();
    getch();
    exitCurses();
    return 1;
}
