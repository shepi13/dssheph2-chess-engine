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
    pieceIcon_destroy(self);

    ILuint pieceID = 0;
    ilGenImages(1, &pieceID);
    ilBindImage(pieceID);

    //perhaps this is bad form???
    if(ilLoadImage(path) == IL_FALSE || ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == IL_FALSE)
    {
        perror("Failed to load piece texture");
        ilDeleteImages(1, &pieceID);
        exit(EXIT_FAILURE); 
    }  

    self->width = (GLuint) ilGetInteger(IL_IMAGE_WIDTH);
    self->height = (GLuint) ilGetInteger(IL_IMAGE_HEIGHT);
    GLuint* data = (GLuint*) ilGetData();

    glGenTextures(1, &(self->ID));
    glBindTexture(GL_TEXTURE_2D, self->ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    
    glBindTexture(GL_TEXTURE_2D, 0);
    ilDeleteImages(1, &pieceID);

    if(glGetError() != GL_NO_ERROR)
    {
        perror("Failed to load texture");
        exit(EXIT_FAILURE);
    }
}

void pieceIcon_draw(pieceIcon* self, GLuint x, GLuint y)
{
    if(self == NULL || self->ID == 0)
        return;

    glTranslatef(x, y, 0.f);

    glBindTexture(GL_TEXTURE_2D, self->ID);

    glBegin(GL_QUADS);
        glTexCoord2f(0.f, 0.f);
        glVertex2f(0.f, 0.f);
    
        glTexCoord2f(1.f, 0.f);
        glVertex2f(SQUAREWIDTH, 0.f);

        glTexCoord2f(1.f, 1.f);
        glVertex2f(SQUAREWIDTH, SQUAREHEIGHT);

        glTexCoord2f(0.f, 1.f);
        glVertex2f(0.f, SQUAREHEIGHT);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}
