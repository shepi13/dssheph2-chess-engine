#include "graphics.h"

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, MYWIDTH, MYHEIGHT, 0.0, 1.0, -1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.85f, 0.85f, 0.85f, 1.f);

    if(glGetError() != GL_NO_ERROR)
    {
        perror("Failed to initialize openGL");
        exit(EXIT_FAILURE);
    }
}

void update() {}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(MYWIDTH/2.f, MYHEIGHT/2.f, 0.0f);

    drawBorder();
    drawBoard();
    drawPieces();

    glutSwapBuffers();
}

void drawBorder() 
{
    glBegin(GL_QUADS);
        glColor3f(0.2f, 0.2f, 0.2f);
        glVertex2f(-BORDERWIDTH/2.f, -BORDERHEIGHT/2.f);
        glVertex2f(-BORDERWIDTH/2.f, BORDERHEIGHT/2.f);
        glVertex2f(BORDERWIDTH/2.f, BORDERHEIGHT/2.f);
        glVertex2f(BORDERWIDTH/2.f, -BORDERHEIGHT/2.f);
    glEnd();
}

void drawBoard() 
{
    glTranslatef(-BOARDWIDTH/2.f, -BOARDHEIGHT/2.f, 0.f);

    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if((x+y) % 2 == 0)
                glColor3f(0.6f, 0.6f, 0.6f);
            else
                glColor3f(0.85f, 0.85f, 0.85f);

            glBegin(GL_QUADS);
                glVertex2f(x*SQUAREWIDTH, y*SQUAREHEIGHT);
                glVertex2f((x+1)*SQUAREWIDTH, y*SQUAREHEIGHT);
                glVertex2f((x+1)*SQUAREWIDTH, (y+1)*SQUAREHEIGHT);
                glVertex2f(x*SQUAREWIDTH, (y+1)*SQUAREHEIGHT);
            glEnd(); 
        }
    } 
}

void drawPieces() {}
