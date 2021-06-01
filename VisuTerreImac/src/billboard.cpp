#include "billboard.h"
using namespace std;

//Prend un nom de texture .png en parametre et retourne la texture associee
GLuint generateTexturePng(char *name)
{
    GLuint textureId;

    SDL_Surface *image = IMG_Load(name);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}

//Dessine un Billboard
void drawBillboard(Vect3f scale)
{
    glRotatef(-90, 1., 0., 0.);
    glRotatef(-90, 0., 0., 1.);

    // Load texture
    char BillboardTexture[] = "./assets/billboards/tree.png";
    unsigned int BillboardTextureture_to_apply = generateTexturePng(BillboardTexture);

    glScalef(scale.x, scale.y, scale.z);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, BillboardTextureture_to_apply);

    glColor3f(1.0f, 1.0f, 1.0f); // set colour
    glBegin(GL_QUADS);

    glTexCoord2f(0., 0.);
    glVertex3f(0., -0.5, 1.);

    glTexCoord2f(1., 0.);
    glVertex3f(0., 0.5, 1.);

    glTexCoord2f(1., 1.);
    glVertex3f(0., 0.5, 0.);

    glTexCoord2f(0., 1.);
    glVertex3f(0., -0.5, 0.);

    glEnd();
}

void addBillboard(Vect3f scale)
{

    float modelview[16];
    int i, j;
    // save the current modelview matrix
    glPushMatrix();
    // get the current modelview matrix
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    // The only difference now is that
    // the i variable will jump over the
    // up vector, 2nd column in OpenGL convention
    for (i = 0; i < 3; i += 2)
        for (j = 0; j < 3; j++)
        {
            if (i == j)
                modelview[i * 4 + j] = 1.0;
            else
                modelview[i * 4 + j] = 0.0;
        }
    // set the modelview matrix with the
    // up vector unchanged
    glLoadMatrixf(modelview);
    drawBillboard(scale);
    // restores the modelview matrix
    glPopMatrix();
}