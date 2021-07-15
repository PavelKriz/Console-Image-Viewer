#include "CApplication.hpp"


void CApplication::initCurses(){
    initscr();
    noecho();
    clear();
}

void CApplication::exitCurses(){
    endwin();
}

CApplication::CApplication(){
    initCurses();
    parsedInput_ = CInputParser::parseInput(argc, argv);
}

int CApplication::run(){
    //init ncurses
    initCurses();
    
    SParsedInput parsedInput;
    unique_ptr<CImage> image;
    try{
        parsedInput = CInputParser::parseInput(argc, argv);
        image = make_unique<CImage>(parsedInput.relativeFilepathToImage_);
    } catch (const invalid_argument& ia){
        initCurses();
        printw(ia.what());
        printw("\nPress any key to end...\n");
        refresh();
        getch();
        exitCurses();
        return 1;
    }
    
    //init ncurses
    initCurses();
    
    int prevCols = 0;
    int prevRows = 0;
    while(true){
        int cols, rows;
        getmaxyx(stdscr, rows, cols);
        
        int width = image->getWidth();
        int height = image->getHeight();
        
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
        
        CImage currentImage((*image), newSizeX, newSizeY);
        
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
