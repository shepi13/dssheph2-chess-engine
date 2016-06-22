#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "myGLIncludes.h"

#define MYWIDTH 720
#define MYHEIGHT 540
#define MYFPS 60
#define BORDERWIDTH (BOARDWIDTH + 10)
#define BORDERHEIGHT (BOARDHEIGHT + 10)
#define SQUAREWIDTH 64
#define SQUAREHEIGHT 64
#define BOARDWIDTH (8*SQUAREWIDTH)
#define BOARDHEIGHT (8*SQUAREHEIGHT)

void init();
void update();
void render();

void drawBorder();
void drawBoard();
void drawPieces();

#endif
