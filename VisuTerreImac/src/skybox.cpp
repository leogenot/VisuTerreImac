#include "skybox.h"
using namespace std;

//pour stocker les textures sur chaque faces et se reperer ez
enum
{
	SKY_LEFT,
	SKY_BACK,
	SKY_RIGHT,
	SKY_FRONT,
	SKY_TOP,
	SKY_BOTTOM
};

 //un id pour chaque texture
unsigned int skybox[6];

unsigned int loadTexture(const char *filename)
{
	unsigned int texture_id;
	glGenTextures(1, &texture_id); //on genere un nom de texture unique
	SDL_Surface *img = IMG_Load(filename);
	if (img == 0)
	{
		printf("Error loading image %s %s", filename, SDL_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}//on charge limage texture en .bmp
	glBindTexture(GL_TEXTURE_2D, texture_id);//on bind la texture 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//si la texture est trop petite on prend la moyenne du pixel a cote
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//pareil si elle est plus grande
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//on duplique la derniere rangée de pixels pour eviter d´avoir une ligne entre les faces du cube horizontales, c´est plus smooth
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//pareil mais verticalement
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, img->pixels); //on cree la texture finale
	SDL_FreeSurface(img);//on supprime l´image qui n´est plus utile

	return texture_id;


}

//on charge toutes les textures dans la skybox (chaque face)
void initskybox()
{
	skybox[SKY_LEFT] = loadTexture("./assets/skybox/left.bmp");
	skybox[SKY_BACK] = loadTexture("./assets/skybox/back.bmp");
	skybox[SKY_RIGHT] = loadTexture("./assets/skybox/right.bmp");
	skybox[SKY_FRONT] = loadTexture("./assets/skybox/front.bmp");
	skybox[SKY_TOP] = loadTexture("./assets/skybox/top.bmp");
	skybox[SKY_BOTTOM] = loadTexture("./assets/skybox/bottom.bmp");
}

//supprime la box pour pas avoir d´erreur de memoire
void killskybox()
{
	glDeleteTextures(6, &skybox[0]);
}

void drawSkybox(float size)
{
	bool turned_on_texture = glIsEnabled(GL_TEXTURE_2D); //verrifier si la texture est deja appliquee
	glDisable(GL_LIGHTING);//on supprime leclairage car on est a linterieur du cube de la skybox
	glEnable(GL_TEXTURE_2D);//on applique la texture:
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_BACK]); 
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS); 
	glTexCoord2f(0, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_FRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_RIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_TOP]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_BOTTOM]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
	if (!turned_on_texture)
		glDisable(GL_TEXTURE_2D);
}
