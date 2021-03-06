# simple makefile for the project

NAME= CIViewer
OBJFILES= src/consoleImg.o src/CImage.o src/CApplication.o src/CInputParser.o src/CLoopTimeManager.o src/CInputHandler.o

CC=g++
LIBS= -lm -lncurses
WARNFLAGS= -pedantic -Wall

#object creating rule
%.o: %.cpp
	$(CC) $(WARNFLAGS) -c $< -o $@

all: $(NAME)
	
dep:
	$(CC) -MM *.cpp >dep.list
	
-include dep.list

$(NAME): $(OBJFILES)
	$(CC) $(OBJFILES) $(LIBS) -o $@
