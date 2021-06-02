#include "TerrainFromQuadtree.h"
using namespace std;

TerrainFromQuadtree::TerrainFromQuadtree(char *terrainFilename, const int width, const int length, float scaleH, float scale, int precisionLevel)
{
	cout << "Creation du terrain" << endl;
	WireframeMode = false;

	for (int x = 0; x <= width + 1; x++)
	{
		terrainData.push_back(vector<Vect3f>());
		for (int z = 0; z <= length + 1; z++)
		{
			terrainData[x].push_back(Vect3f(0.0f, 0.0f, 0.0f));
		}
	}

	cout << "Ouverture de la heightmap: " << terrainFilename << endl;
	opened_file = fopen(terrainFilename, "rb");

	if (opened_file == NULL)
	{
		cout << "ERREUR, pas de heightmap trouvée !" << endl;
		return;
	}

	//Stocke chaque bit du fichier heightmap dans le tableau heightmap
	fread(heightmap, 1, width * length, opened_file);

	fclose(opened_file);

	//instanciation du width et height du terrain
	mapWidth = width;
	mapHeight = length;

	//instanciation du scale du terrain
	scaleHeight = scaleH;
	terrainScale = scale;

	//on met le terrain au centre du repere origine
	adjFromOrig = (terrainScale * mapWidth) / 2;

	//boite  englobante pour le quadtree
	boundary.top = -adjFromOrig;
	boundary.bottom = adjFromOrig;
	boundary.left = -adjFromOrig;
	boundary.right = adjFromOrig;

	generateTerrainPoints();
	calculateCellBoundary();

	terrainQuadtree = new TerrainQuadTree(boundary.top, boundary.bottom, boundary.left, boundary.right, mapWidth, mapHeight, precisionLevel);

	LOD_range = (width * scale) / 2;
	cout << "Valeur du LOD : " << LOD_range << endl;
}

//destructeur de la classe
TerrainFromQuadtree::~TerrainFromQuadtree()
{
	delete terrainQuadtree;
	cout << "Terrain et quadtree supprimé" << endl;
}

void TerrainFromQuadtree::printTerrainData() // print le terrain dans la console (fait crasher suivnt la taille du terrain)
{
	cout << "Tous les points du terrain :" << endl;

	for (int x = 0; x < mapWidth; x++) // x
	{
		for (int z = 0; z < mapHeight; z++) // z
		{
			if ((float)heightmap[x][z] == 0)
				cout << "NULL, terrain vide";
			else
				cout << (float)heightmap[x][z] << " ";
		}
		cout << endl;
	}
}

//Prend un nom de texture .jpg en parametre et retourne la texture associee
GLuint TerrainFromQuadtree::generateTextureJpg(char *name)
{
	GLuint textureId;

	SDL_Surface *image = IMG_Load(name);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

void TerrainFromQuadtree::render(camera &cam)
{
	//cout << "Rendu du terrain" << endl;
	// chargement des texture
	char grassTex[] = "./assets/textures/grass.jpg";
	GLuint Grasstexture_to_apply = generateTextureJpg(grassTex);
	char rockTex[] = "./assets/textures/rock.jpg";
	GLuint Rocktexture_to_apply = generateTextureJpg(rockTex);
	char waterTex[] = "./assets/textures/water.jpg";
	GLuint Watertexture_to_apply = generateTextureJpg(waterTex);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	// je sais meme pas si ca sert vraiment
	GLfloat mShininess[] = {8};
	GLfloat DiffuseMaterial[] = {1.0, 1.0, 1.0};
	GLfloat AmbientMaterial[] = {0.5, 0.5, 0.5};
	GLfloat SpecularMaterial[] = {1.0, 1.0, 1.0};

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, AmbientMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SpecularMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);

	// LOD avec position de la camera
	terrainQuadtree->resetNodeVisibility();
	terrainQuadtree->testRenderable(terrainQuadtree->NodeArray[0], Vect3f(cam.cam_coord.x, cam.cam_coord.y, cam.cam_coord.z), LOD_range);

	// dessin du repère dans le monde
	glBegin(GL_LINES);
	glColor3f(1., 0., 0.);
	glVertex3f(0., 0., 0.);
	glVertex3f(100, 0., 0.);
	glColor3f(0., 1., 0.);
	glVertex3i(0., 0., 0.);
	glVertex3i(0., 100, 0.);
	glColor3f(0., 0., 1.);
	glVertex3i(0., 0., 0.);
	glVertex3i(0., 0., 100);
	glEnd();

	//declaration des vertices pour le triangle strip
	int vX1, vX2, vX3, vX4;
	int vZ1, vZ2, vZ3, vZ4;



	//on teste chaque node du quad avec la position de la camera et on verrifie si cest visible
	for (int i = 0; i < terrainQuadtree->nodeSize; i++)
	{

		if (terrainQuadtree->NodeArray[i].visible == true)
		{
			const int quadSize = 2;

			//mode wireframe ou plein du terrain
			if (WireframeMode)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glColor3f(1.0f, 1.0f, 1.0f);

			// maintenant on check les quad plutot que chaque triangle car le quad utilise des carres composé de deux triangles
			for (int x = 0; x < quadSize; x++)
			{

				for (int z = 0; z < quadSize; z++)
				{
					// les vertices de chaque triangle pour construire le terrain
					vX1 = terrainQuadtree->NodeArray[i].verticeIndex[x][z].x;
					vZ1 = terrainQuadtree->NodeArray[i].verticeIndex[x][z].z;
					vX2 = terrainQuadtree->NodeArray[i].verticeIndex[x][z + 1].x;
					vZ2 = terrainQuadtree->NodeArray[i].verticeIndex[x][z + 1].z;
					vX3 = terrainQuadtree->NodeArray[i].verticeIndex[x + 1][z].x;
					vZ3 = terrainQuadtree->NodeArray[i].verticeIndex[x + 1][z].z;
					vX4 = terrainQuadtree->NodeArray[i].verticeIndex[x + 1][z + 1].x;
					vZ4 = terrainQuadtree->NodeArray[i].verticeIndex[x + 1][z + 1].z;

					Vect3f p1 = terrainData[vX1][vZ1];
					Vect3f p2 = terrainData[vX2][vZ2];
					Vect3f p3 = terrainData[vX3][vZ3];
					Vect3f p4 = terrainData[vX4][vZ4];

					//raycasting sur le terrain, on check l´intersection
					bool intersection = cam.IntersectRayTriangle(p1, p2, p3).succeeded || cam.IntersectRayTriangle(p2, p3, p4).succeeded;

					// on applique les textures en fonction de la heuteur du terrain
					if (terrainData[vX1][vZ1].y > scaleHeight && terrainData[vX2][vZ2].y > scaleHeight && terrainData[vX3][vZ3].y > scaleHeight && terrainData[vX4][vZ4].y > scaleHeight)
						glBindTexture(GL_TEXTURE_2D, Rocktexture_to_apply);
					else if (terrainData[vX1][vZ1].y > scaleHeight / 2 && terrainData[vX2][vZ2].y > scaleHeight / 2 && terrainData[vX3][vZ3].y > scaleHeight / 2 && terrainData[vX4][vZ4].y > scaleHeight / 2)
						glBindTexture(GL_TEXTURE_2D, Grasstexture_to_apply);
					else
						glBindTexture(GL_TEXTURE_2D, Watertexture_to_apply);


					glBegin(GL_TRIANGLE_STRIP);

					if (intersection)
					{

						//cout << "INTERSECTION" << endl;
						const Uint8 *state = SDL_GetKeyboardState(NULL);
						//augmenter la heuteur d´un quad a l´appui de la touche U
						SDL_PumpEvents();
						if (state[SDL_SCANCODE_Y])
						{
							terrainData[vX1][vZ1].y += scaleHeight / 10;
							terrainData[vX2][vZ2].y += scaleHeight / 10;
							terrainData[vX3][vZ3].y += scaleHeight / 10;
							terrainData[vX4][vZ4].y += scaleHeight / 10;
						}
						else
						{
							//coloration en rouge du quad intersecté
							glColor3f(1, 0, 0);
						}
					}

					else
					{
						glTexCoord2f(0, 0);
						glVertex3f(p1.x, p1.y, p1.z);

						glTexCoord2f(0, 1);
						glVertex3f(p2.x, p2.y, p2.z);

						glTexCoord2f(1, 0);
						glVertex3f(p3.x, p3.y, p3.z);

						glTexCoord2f(1, 1);
						glVertex3f(p4.x, p4.y, p4.z);
					}

					glTexCoord2f(0, 0);
					glVertex3f(p1.x, p1.y, p1.z);

					glTexCoord2f(0, 1);
					glVertex3f(p2.x, p2.y, p2.z);

					glTexCoord2f(1, 0);
					glVertex3f(p3.x, p3.y, p3.z);

					glTexCoord2f(1, 1);
					glVertex3f(p4.x, p4.y, p4.z);
				}

				glEnd();
			}
		}
	}
}

// on stocke tous les points de chaque vertices du terrain
void TerrainFromQuadtree::generateTerrainPoints()
{
	cout << "Generation des points du terrain" << endl;
	// x = ligne z = colonne
	for (int x = 0; x < mapWidth; x++)
	{
		for (int z = 0; z < mapHeight; z++)
		{
			terrainData[x][z] = Vect3f(x * terrainScale - adjFromOrig, (heightmap[x][z] / 255.0) * scaleHeight * terrainScale, z * terrainScale - adjFromOrig);
		}
	}
	// recuperer la hauteur en un point du terrain : (heightmap[x][z] / 255.0) * scaleHeight * terrainScale ou terrainData[x][z].y
}

float TerrainFromQuadtree::getTerrainHeight(Vect3f pos)
{
	return terrainData[pos.x][pos.z].y;
}
//calculer les quatres cotés de la cellule englobante
void TerrainFromQuadtree::calculateCellBoundary()
{
	cout << "Calcul des cellules englobantes" << endl;

	for (int x = 0; x < mapWidth; x++)
	{
		for (int z = 0; z < mapHeight; z++)
		{
			quadboundary[x][z].top = z * terrainScale - adjFromOrig;
			quadboundary[x][z].bottom = (z + 1) * terrainScale - adjFromOrig;
			quadboundary[x][z].left = x * terrainScale - adjFromOrig;
			quadboundary[x][z].right = (x + 1) * terrainScale - adjFromOrig;
		}
	}
}

// on check si un point est dans la cellule englobante
bool TerrainFromQuadtree::withinBoundary(Vect3f pos, QuadBoundary boundary)
{
	if ((pos.x > boundary.left) && (pos.x < boundary.right))
		if ((pos.z > boundary.top) && (pos.z < boundary.bottom))
			return true;
		else
			return false;
}

//on set la valeur du LOD (voir plus ou moins de detail au loin)
void TerrainFromQuadtree::setLOD_range(float value)
{
	LOD_range += value;
}

//toggle du mode wireframe
void TerrainFromQuadtree::setWireframe()
{
	WireframeMode = !WireframeMode;
}
