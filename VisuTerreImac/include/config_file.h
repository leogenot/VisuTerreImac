#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>

struct Config
{
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    float field_of_view;
    char heightmap_file[64];
    int xsize;
    int ysize;
    int zmin;
    int zmax;
    float znear;
    float zfar;
    float terrain_Scale;
    float terrainHeightScale;
    int terrain_Precision;

};

void loadConfig(Config &config);

#endif