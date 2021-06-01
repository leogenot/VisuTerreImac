#ifndef TERRAINFROMQUADTREE_H
#define TERRAINFROMQUADTREE_H

#include "TerrainQuadTree.h"
#include "Vect3f.h"
#include "config_file.h"
#include "camera.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include "billboard.h"

using namespace std;

//boite englobante
struct QuadBoundary
{
	float top;
	float bottom;
	float left;
	float right;
};

class TerrainFromQuadtree
{
private:
	bool WireframeMode;

	uint8_t heightmap[512][512]; //stockage des infos de la heightmap ppm

	QuadBoundary quadboundary[512][512]; //tableau de chaque boite englobante du terrain
	QuadBoundary boundary;				 //boite englobante de tout le terrrain

	FILE *opened_file;
	float scaleHeight;
	float terrainScale;
	float adjFromOrig; //remettre a ucentre du repere opengl

	int mapWidth, mapHeight; //width et height en pixel du terrain
	float LOD_range;

public:
	TerrainFromQuadtree() {}
	TerrainFromQuadtree(char *terrainFilename, const int width, const int length, float scaleH, float scale, int precisionLevel);
	~TerrainFromQuadtree();

	TerrainQuadTree *terrainQuadtree;

	vector<vector<Vect3f>> terrainData; //coords du terrain

	GLuint generateTextureJpg(char *name);
	void printTerrainData();
	void render(camera &cam);
	void generateTerrainPoints();
	float getTerrainHeight(Vect3f pos);
	void calculateCellBoundary();
	bool withinBoundary(Vect3f pos, QuadBoundary bounds);
	void setLOD_range(float value);
	void setWireframe();
};

#endif
