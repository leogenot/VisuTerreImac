#include <iostream>
#include "TerrainQuadTree.h"

using namespace std;

TerrainQuadTree::TerrainQuadTree(float Top, float Bottom, float Left, float Right, unsigned int VertX, unsigned int VertZ, unsigned int Level)
{
	cout << "Creation du quadtree" << endl;

	// calcul de la taill des nodes pour eviter les leaks de memoire
	nodeSize = calculateNbNodes(Level);
	NodeArray = (QuadtreeNode *)malloc(sizeof(QuadtreeNode) * nodeSize);


	// on calcule la taille du quad minimum pour determiner si cest une leaf ou si on dessine un enfant
	minSizeOfQuad = calculateMinQuadSize(Bottom - Top, Level);


	QuadtreeNode firstNode; //premiere node


	firstNode.top = Top;
	firstNode.bottom = Bottom;
	firstNode.left = Left;
	firstNode.right = Right;

	//on set des ID pour connaitre les etages du quad (level)
	firstNode.ID = 0;
	firstNode.parentID = 0;
	firstNode.layerID = 1;

	//on set les vertices a 0 pour init
	firstNode.VertexInitX = 0; 
	firstNode.VertexInitZ = 0;

	// nombre de vertices sur x et z (en accord avec la taille de la map passée en param)
	vertX = VertX;
	vertZ = VertZ;

	createQuadTree(firstNode);
	adjustVerticeIndex();
}

//calcul de la taille des nodes du quadtree
unsigned int TerrainQuadTree::calculateNbNodes(unsigned int Level)
{
	cout << "Calcul du nombre de nodes du quadtree" << endl;
	unsigned int numNodes = 0;

	for (int i = 0; i < Level; i++)
	{
		numNodes += pow(4, i);
		cout << "Niveau [" << i << "] nb total de node au niveau actuel: " << numNodes << endl;
	}

	cout << "Nombre total de node du quadtree: " << numNodes << endl;

	return numNodes;
}

// on calcule la taille du quad minimum pour determiner si cest une leaf ou si on dessine un enfant
float TerrainQuadTree::calculateMinQuadSize(float Width, unsigned int Level)
{
	float size = Width;

	for (int i = 0; i < Level - 1; i++)
	{
		size *= 0.5;
	}

	return size;
}

void TerrainQuadTree::createQuadTree(QuadtreeNode &ActualNode)
{
	static int nodeIndex;
	unsigned int TypeOfCurrentNode;

	// on check si c´est une leaf ou cest encore une node
	if ((ActualNode.bottom - ActualNode.top) == minSizeOfQuad)
	{
		TypeOfCurrentNode = IsLeaf;
	}
	else
	{
		TypeOfCurrentNode = IsNode;
	}

	// creation du node actuel ( en fonction du resultat precedent)
	QuadtreeNode *PointerOnNode = &NodeArray[ActualNode.ID];

	PointerOnNode->parentID = ActualNode.parentID;
	PointerOnNode->ID = ActualNode.ID;
	PointerOnNode->nodeType = TypeOfCurrentNode;
	PointerOnNode->visible = false;

	//on assigne la taille du node
	PointerOnNode->width = ActualNode.bottom - ActualNode.top;
	PointerOnNode->height = ActualNode.right - ActualNode.left;

	//on assigne a boite englobante du node
	PointerOnNode->top = ActualNode.top;
	PointerOnNode->bottom = ActualNode.bottom;
	PointerOnNode->left = ActualNode.left;
	PointerOnNode->right = ActualNode.right;

	//on centre le node
	PointerOnNode->position.x = ((ActualNode.left + ActualNode.right) / 2);
	PointerOnNode->position.z = ((ActualNode.top + ActualNode.bottom) / 2);

	//on recup la position de la node actuelle en fonction de son niveau dans l´arbre
	int divs = (vertX / pow(2, ActualNode.layerID));

	for (int x = 0; x < 3; x++)
	{
		for (int z = 0; z < 3; z++)
		{
			PointerOnNode->verticeIndex[x][z].x = ActualNode.VertexInitX + (divs * x);
			PointerOnNode->verticeIndex[x][z].z = ActualNode.VertexInitZ + (divs * z);
		}
	}

	//creation des enfants
	if (TypeOfCurrentNode == IsLeaf)
	{
		return;
	}
	else
	{
		//on cree un nouveau quadtree pour contenir les enfants
		QuadtreeNode childNode;

		childNode.width = PointerOnNode->width / 2;
		childNode.height = PointerOnNode->height / 2;

		//Top left quad
		nodeIndex++;
		PointerOnNode->branchIndex[0] = nodeIndex;

		childNode.top = PointerOnNode->top;
		childNode.bottom = PointerOnNode->top + childNode.height;
		childNode.left = PointerOnNode->left;
		childNode.right = PointerOnNode->left + childNode.width;
		childNode.parentID = ActualNode.ID;
		childNode.ID = nodeIndex;

		childNode.layerID = ActualNode.layerID + 1;
		childNode.VertexInitX = ActualNode.VertexInitX;
		childNode.VertexInitZ = ActualNode.VertexInitZ;

		createQuadTree(childNode);

		// Top right quad
		nodeIndex++;
		PointerOnNode->branchIndex[1] = nodeIndex;

		childNode.top = PointerOnNode->top;
		childNode.bottom = PointerOnNode->top + childNode.height;
		childNode.left = PointerOnNode->right - childNode.width;
		childNode.right = childNode.left + childNode.width;
		childNode.parentID = ActualNode.ID;
		childNode.ID = nodeIndex;

		childNode.layerID = ActualNode.layerID + 1;			   
		childNode.VertexInitX = ActualNode.VertexInitX + divs;
		childNode.VertexInitZ = ActualNode.VertexInitZ;		  

		createQuadTree(childNode);

		// Bottom left quad
		nodeIndex++;
		PointerOnNode->branchIndex[2] = nodeIndex;

		childNode.top = PointerOnNode->bottom - childNode.height;
		childNode.bottom = childNode.top + childNode.height;
		childNode.left = PointerOnNode->left;
		childNode.right = PointerOnNode->left + childNode.width;
		childNode.parentID = ActualNode.ID;
		childNode.ID = nodeIndex;

		childNode.layerID = ActualNode.layerID + 1;			   
		childNode.VertexInitX = ActualNode.VertexInitX;		  
		childNode.VertexInitZ = ActualNode.VertexInitZ + divs; 

		createQuadTree(childNode);

		// Bottom right quad
		nodeIndex++;
		PointerOnNode->branchIndex[3] = nodeIndex;

		childNode.top = PointerOnNode->bottom - childNode.height;
		childNode.bottom = childNode.top + childNode.height;
		childNode.left = PointerOnNode->right - childNode.width;
		childNode.right = childNode.left + childNode.width;
		childNode.parentID = ActualNode.ID;
		childNode.ID = nodeIndex;

		childNode.layerID = ActualNode.layerID + 1;			   
		childNode.VertexInitX = ActualNode.VertexInitX + divs;
		childNode.VertexInitZ = ActualNode.VertexInitZ + divs;

		createQuadTree(childNode);
	}
}

// on ajuste les coord des dernieres vertices sinon gros bug on sort du tableau (merci stack overflow)
void TerrainQuadTree::adjustVerticeIndex()
{
	for (int i = 0; i < nodeSize; i++)
	{
		for (int x = 0; x < 3; x++)
		{
			for (int z = 0; z < 3; z++)
			{

				if (NodeArray[i].verticeIndex[x][z].x > 0)
				{
					NodeArray[i].verticeIndex[x][z].x = NodeArray[i].verticeIndex[x][z].x - 1;
				}

				if (NodeArray[i].verticeIndex[x][z].z > 0)
				{
					NodeArray[i].verticeIndex[x][z].z = NodeArray[i].verticeIndex[x][z].z - 1;
				}
			}
		}
	}
}

//on set la visibilité de la node a faux  (pour init le terrain par ex)
void TerrainQuadTree::resetNodeVisibility()
{
	for (int i = 0; i < nodeSize; i++)
	{
		NodeArray[i].visible = false;
	}
}


// LOD, on teste la position des nodes par rappor a la camera
void TerrainQuadTree::testRenderable(QuadtreeNode &parentNode, Vect3f pos, float range)
{

	if (parentNode.nodeType != IsLeaf)
	{
		//check si on render les child nodes
		bool NeedToRenderChild1 = false;
		bool NeedToRenderChild2 = false;
		bool NeedToRenderChild3 = false;
		bool NeedToRenderChild4 = false;

		float TopLeftX = NodeArray[parentNode.branchIndex[0]].left - pos.x;
		float TopLeftZ = NodeArray[parentNode.branchIndex[0]].top - pos.z;
		float TopLeftY = sqrt((TopLeftX * TopLeftX) + (TopLeftZ * TopLeftZ));

		float TopRightX = NodeArray[parentNode.branchIndex[0]].right - pos.x;
		float TopRightZ = NodeArray[parentNode.branchIndex[0]].top - pos.z;
		float TopRightY = sqrt((TopRightX * TopRightX) + (TopRightZ * TopRightZ));

		float BottomLeftX = NodeArray[parentNode.branchIndex[0]].left - pos.x;
		float BottomLeftZ = NodeArray[parentNode.branchIndex[0]].bottom - pos.z;
		float BottomLeftY = sqrt((BottomLeftX * BottomLeftX) + (BottomLeftZ * BottomLeftZ));

		float BottomRightX = NodeArray[parentNode.branchIndex[0]].right - pos.x;
		float BottomRightZ = NodeArray[parentNode.branchIndex[0]].bottom - pos.z;
		float BottomRightY = sqrt((BottomRightX * BottomRightX) + (BottomRightZ * BottomRightZ));

		//si on est < range on le render, on check ses 4 angles
		if ((TopLeftY < range) || (TopRightY < range) || (BottomLeftY < range) || (BottomRightY < range))
		{
			NeedToRenderChild1 = true;
			NodeArray[parentNode.branchIndex[0]].visible = false;
			testRenderable(NodeArray[parentNode.branchIndex[0]], pos, range);

		}
		else
		{
			NodeArray[parentNode.branchIndex[0]].visible = true;

		}

		TopLeftX = NodeArray[parentNode.branchIndex[1]].left - pos.x;
		TopLeftZ = NodeArray[parentNode.branchIndex[1]].top - pos.z;
		TopLeftY = sqrt((TopLeftX * TopLeftX) + (TopLeftZ * TopLeftZ));

		TopRightX = NodeArray[parentNode.branchIndex[1]].right - pos.x;
		TopRightZ = NodeArray[parentNode.branchIndex[1]].top - pos.z;
		TopRightY = sqrt((TopRightX * TopRightX) + (TopRightZ * TopRightZ));

		BottomLeftX = NodeArray[parentNode.branchIndex[1]].left - pos.x;
		BottomLeftZ = NodeArray[parentNode.branchIndex[1]].bottom - pos.z;
		BottomLeftY = sqrt((BottomLeftX * BottomLeftX) + (BottomLeftZ * BottomLeftZ));

		BottomRightX = NodeArray[parentNode.branchIndex[1]].right - pos.x;
		BottomRightZ = NodeArray[parentNode.branchIndex[1]].bottom - pos.z;
		BottomRightY = sqrt((BottomRightX * BottomRightX) + (BottomRightZ * BottomRightZ));

		if ((TopLeftY < range) || (TopRightY < range) || (BottomLeftY < range) || (BottomRightY < range))
		{
			NeedToRenderChild2 = true;
			NodeArray[parentNode.branchIndex[1]].visible = false;
			testRenderable(NodeArray[parentNode.branchIndex[1]], pos, range);
		}
		else
		{
			NodeArray[parentNode.branchIndex[1]].visible = true;
		}

		TopLeftX = NodeArray[parentNode.branchIndex[2]].left - pos.x;
		TopLeftZ = NodeArray[parentNode.branchIndex[2]].top - pos.z;
		TopLeftY = sqrt((TopLeftX * TopLeftX) + (TopLeftZ * TopLeftZ));

		TopRightX = NodeArray[parentNode.branchIndex[2]].right - pos.x;
		TopRightZ = NodeArray[parentNode.branchIndex[2]].top - pos.z;
		TopRightY = sqrt((TopRightX * TopRightX) + (TopRightZ * TopRightZ));

		BottomLeftX = NodeArray[parentNode.branchIndex[2]].left - pos.x;
		BottomLeftZ = NodeArray[parentNode.branchIndex[2]].bottom - pos.z;
		BottomLeftY = sqrt((BottomLeftX * BottomLeftX) + (BottomLeftZ * BottomLeftZ));

		BottomRightX = NodeArray[parentNode.branchIndex[2]].right - pos.x;
		BottomRightZ = NodeArray[parentNode.branchIndex[2]].bottom - pos.z;
		BottomRightY = sqrt((BottomRightX * BottomRightX) + (BottomRightZ * BottomRightZ));


		if ((TopLeftY < range) || (TopRightY < range) || (BottomLeftY < range) || (BottomRightY < range))
		{
			NeedToRenderChild3 = true;
			NodeArray[parentNode.branchIndex[2]].visible = false;
			testRenderable(NodeArray[parentNode.branchIndex[2]], pos, range);
		}
		else
		{
			NodeArray[parentNode.branchIndex[2]].visible = true;
		}

		TopLeftX = NodeArray[parentNode.branchIndex[3]].left - pos.x;
		TopLeftZ = NodeArray[parentNode.branchIndex[3]].top - pos.z;
		TopLeftY = sqrt((TopLeftX * TopLeftX) + (TopLeftZ * TopLeftZ));

		TopRightX = NodeArray[parentNode.branchIndex[3]].right - pos.x;
		TopRightZ = NodeArray[parentNode.branchIndex[3]].top - pos.z;
		TopRightY = sqrt((TopRightX * TopRightX) + (TopRightZ * TopRightZ));

		BottomLeftX = NodeArray[parentNode.branchIndex[3]].left - pos.x;
		BottomLeftZ = NodeArray[parentNode.branchIndex[3]].bottom - pos.z;
		BottomLeftY = sqrt((BottomLeftX * BottomLeftX) + (BottomLeftZ * BottomLeftZ));

		BottomRightX = NodeArray[parentNode.branchIndex[3]].right - pos.x;
		BottomRightZ = NodeArray[parentNode.branchIndex[3]].bottom - pos.z;
		BottomRightY = sqrt((BottomRightX * BottomRightX) + (BottomRightZ * BottomRightZ));

		if ((TopLeftY < range) || (TopRightY < range) || (BottomLeftY < range) || (BottomRightY < range))
		{
			NeedToRenderChild4 = true;
			NodeArray[parentNode.branchIndex[3]].visible = false;
			testRenderable(NodeArray[parentNode.branchIndex[3]], pos, range); 
		}
		else
		{
			NodeArray[parentNode.branchIndex[3]].visible = true;
		}

		//si tous les points de lenfant sont visible alors le parent est set a non visible 
		if ((NeedToRenderChild1 == true) || (NeedToRenderChild2 == true) || (NeedToRenderChild3 == true) || (NeedToRenderChild4 == true))
		{
			parentNode.visible = false;
		}
	}
	else
	{
		//si cest une leaf cest visible
		parentNode.visible = true;
	}
}
