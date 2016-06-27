#include "glBoard.h"
#include "pieceIcon.h"

pieceIcon pawnW, pawnB;
pieceIcon rookW, rookB;
pieceIcon bishopW, bishopB;
pieceIcon knightW, knightB;
pieceIcon queenW, queenB;
pieceIcon kingW, kingB;

pieceIcon* currentPiece = NULL;

void setupGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, MYWIDTH, MYHEIGHT, 0.0, 1.0, -1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.85f, 0.85f, 0.85f, 1.f);

    glEnable(GL_TEXTURE_2D);

    if(glGetError() != GL_NO_ERROR)
    {
        perror("Failed to initialize openGL");
        exit(EXIT_FAILURE);
    }
}

void setupIL()
{
    ilInit();
    ilClearColor(200, 255, 255, 000);

    if(ilGetError() != IL_NO_ERROR)
    {
        perror("Failed to initialize openGL");
        exit(EXIT_FAILURE);
    }
}

void loadPieces()
{
    atexit(cleanUp);

    pieceIcon_create(&pawnW);
    pieceIcon_create(&pawnB);
    pieceIcon_create(&knightW);
    pieceIcon_create(&knightB);
    pieceIcon_create(&queenW);
    pieceIcon_create(&queenB);
    pieceIcon_create(&kingW);
    pieceIcon_create(&kingB);
    pieceIcon_create(&rookW);
    pieceIcon_create(&rookB);
    pieceIcon_create(&bishopW);
    pieceIcon_create(&bishopB);

    //TODO:
    //replace individual images with a sprite sheet.
    pieceIcon_load(&pawnW, "images/pawnW.png");
    pieceIcon_load(&pawnB, "images/pawnB.png");
    pieceIcon_load(&rookW, "images/rookW.png");
    pieceIcon_load(&rookB, "images/rookB.png");
    pieceIcon_load(&bishopW, "images/bishopW.png");
    pieceIcon_load(&bishopB, "images/bishopB.png");
    pieceIcon_load(&knightW, "images/knightW.png");
    //This file not made yet!!! pieceIcon_load(&knightB, "images/knightB.png");
    pieceIcon_load(&queenW, "images/queenW.png");
    pieceIcon_load(&queenB, "images/queenB.png");
    pieceIcon_load(&kingW, "images/kingW.png");
    pieceIcon_load(&kingB, "images/kingB.png");
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawBorder();
    drawBoard();

    glutSwapBuffers();
}

void drawBorder() 
{
    glLoadIdentity();
    glTranslatef(MYWIDTH/2.f, MYHEIGHT/2.f, 0.0f);

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
    for(int x = 0; x < 8; x++)
    {
        for(int y = 0; y < 8; y++)
        {
            if((x+y) % 2 == 0)
                glColor3f(0.85f, 0.85f, 0.85f);
            else
                glColor3f(0.65f, 0.65f, 0.65f);

            glLoadIdentity();
            glTranslatef((MYWIDTH-BOARDWIDTH)/2.f, (MYHEIGHT-BOARDHEIGHT)/2.f, 0.f);

            glBegin(GL_QUADS);
                glVertex2f(x*SQUAREWIDTH, y*SQUAREHEIGHT);
                glVertex2f((x+1)*SQUAREWIDTH, y*SQUAREHEIGHT);
                glVertex2f((x+1)*SQUAREWIDTH, (y+1)*SQUAREHEIGHT);
                glVertex2f(x*SQUAREWIDTH, (y+1)*SQUAREHEIGHT);
            glEnd(); 

            glColor3f(1.f, 1.f, 1.f);
            pieceIcon_draw(&pawnB, x*SQUAREWIDTH, y*SQUAREHEIGHT);
        }
    } 
}

void keyHandler(unsigned char key, int x, int y)
{

}

void mouseHandler(int button, int state, int x, int y)
{

}

void cleanUp()
{
    pieceIcon_destroy(&pawnW);
    pieceIcon_destroy(&pawnB);
    pieceIcon_destroy(&knightW);
    pieceIcon_destroy(&knightB);
    pieceIcon_destroy(&queenW);
    pieceIcon_destroy(&queenB);
    pieceIcon_destroy(&kingW);
    pieceIcon_destroy(&kingB);
    pieceIcon_destroy(&rookW);
    pieceIcon_destroy(&rookB);
    pieceIcon_destroy(&bishopW);
    pieceIcon_destroy(&bishopB);
}
