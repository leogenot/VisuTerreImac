#ifndef TERRAINQUADTREE_H
#define TERRAINQUADTREE_H

//#include "OGLUtil.h"
#include "stdlib.h"
#include "math.h"
#include "Vect3f.h"

struct IndexInfoVertice { int x, z; };// pour les coord de terrainData[x][z]
enum NodeType {IsNode, IsLeaf};

struct QuadtreeNode
{
	int nodeType;
	int ID;
	int parentID;
	float top, bottom, left, right;
	float width, height;//taille de la boite englobante (parente)
	Vect3f position;
	bool visible;


	unsigned int branchIndex[4];

	IndexInfoVertice verticeIndex[3][3];

	int layerID;

	int VertexInitX, VertexInitZ;


};

class TerrainQuadTree
{
private:
	unsigned int leafNodesIndex;
	float minSizeOfQuad;
public:
	TerrainQuadTree();
	TerrainQuadTree(float Top, float Bottom, float Left, float Right, unsigned int VertX, unsigned int VertZ, unsigned int Level);

	unsigned int vertX;
	unsigned int vertZ;
	unsigned int nodeSize;
	QuadtreeNode *NodeArray;

	unsigned int calculateNbNodes(unsigned int Level);
	float calculateMinQuadSize(float Width, unsigned int Level);
	void createQuadTree(QuadtreeNode &ActualNode);
	void adjustVerticeIndex();
	void resetNodeVisibility();
	void testRenderable(QuadtreeNode &parentNode, Vect3f pos, float range);
};

#endif
