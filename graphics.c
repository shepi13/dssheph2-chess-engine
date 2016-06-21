#include "graphics.h"

const int MYWIDTH = 640;
const int MYHEIGHT = 640;
const int MYFPS = 60;

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.f, 0.f, 0.f, 1.f);

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

    glutSwapBuffers();
}
