#include "config_file.h"
using namespace std;

void loadConfig(Config &config)
{
    ifstream opened_file("config_file.timac");
    string line;
    while (getline(opened_file, line))
    {
        istringstream read(line.substr(line.find("=") + 1));
        if (line.find("WINDOW_WIDTH") != -1)
            read >> config.WINDOW_WIDTH;
        else if (line.find("WINDOW_HEIGHT") != -1)
            read >> config.WINDOW_HEIGHT;
        else if (line.find("field_of_view") != -1)
            read >> config.field_of_view;
        else if (line.find("heightmap_file") != -1)
            read >> config.heightmap_file;
        else if (line.find("xsize") != -1)
            read >> config.xsize;
        else if (line.find("ysize") != -1)
            read >> config.ysize;
        else if (line.find("zmin") != -1)
            read >> config.zmin;
        else if (line.find("zmax") != -1)
            read >> config.zmax;
        else if (line.find("znear") != -1)
            read >> config.znear;
        else if (line.find("zfar") != -1)
            read >> config.zfar;
        else if (line.find("terrain_Scale") != -1)
            read >> config.terrain_Scale;
        else if (line.find("terrainHeightScale") != -1)
            read >> config.terrainHeightScale;
        else if (line.find("terrain_Precision") != -1)
            read >> config.terrain_Precision;
            

    }
}
