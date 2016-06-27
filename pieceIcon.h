#ifndef PIECEICON_H
#define PIECEICON_H
#include "myGLIncludes.h"

struct pieceIconStruct
{
    GLuint ID;
    GLuint width;
    GLuint height;
};

typedef struct pieceIconStruct pieceIcon;

void pieceIcon_create(pieceIcon* self);
void pieceIcon_destroy(pieceIcon* self);
void pieceIcon_draw(pieceIcon* self);
void pieceIcon_load(pieceIcon* self, char* path);

#endif
