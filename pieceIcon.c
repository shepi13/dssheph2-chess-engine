#include "pieceIcon.h"

void pieceIcon_create(pieceIcon* self)
{
    self->ID = 0;
    self->width = 0;
    self->height = 0;
}

void pieceIcon_destroy(pieceIcon* self)
{
    if(self->ID == 0)
        return;

    glDeleteTextures(1, &(self->ID));
    self->ID = 0;
    self->width = 0;
    self->height = 0;
}

void pieceIcon_load(pieceIcon* self, char* path)
{

}

void pieceIcon_draw(pieceIcon* self)
{

}
