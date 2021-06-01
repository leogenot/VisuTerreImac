#ifndef BILLBOARD_H
#define BILLBOARD_H
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "Vect3f.h"
#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_access.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>


/*
** Function to load a Jpeg file.
*/
unsigned int LoadTexture(char *textureFile);

GLuint generateTexturePng(char* name);

void drawBillboard(Vect3f scale);

void addBillboard(Vect3f scale);

#endif
