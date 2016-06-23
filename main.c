#include "graphics.h"

void gameLoop(int);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitContextVersion(2, 1);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(MYWIDTH, MYHEIGHT);
    glutCreateWindow("Chess Engine");

    init();
    glutDisplayFunc(draw);
    glutTimerFunc(1000/MYFPS, gameLoop, 0);

    glutMainLoop();

    return 0;
}

void gameLoop(int param)
{
    draw(); 
    glutTimerFunc(1000/MYFPS, gameLoop, param);
}
