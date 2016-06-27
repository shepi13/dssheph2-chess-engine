#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "myGLIncludes.h"

#define MYWIDTH 900
#define MYHEIGHT 720
#define MYFPS 60
#define BORDERWIDTH (BOARDWIDTH + 30)
#define BORDERHEIGHT (BOARDHEIGHT + 30)
#define SQUAREWIDTH 70
#define SQUAREHEIGHT 70
#define BOARDWIDTH (8*SQUAREWIDTH)
#define BOARDHEIGHT (8*SQUAREHEIGHT)

void setupGL();
void setupIL();
void draw();
void loadPieces();
void keyHandler(unsigned char key, int x, int y);
void mouseHandler(int button, int state, int x, int y);

void drawBorder();
void drawBoard();
void drawPieces();

#endif
