#ifndef SKYBOX_H
#define SKYBOX_H
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL2/SDL_image.h>
#include <iostream>


//charger le fichier texture
unsigned int loadTexture(const char *filename);

//charger toutes les textures dans la skybox
void initskybox();

//supprimer les textures pour liberer la memoire de la skybox
void killskybox();

//afficher la skybox
void drawSkybox(float size);


#endif
