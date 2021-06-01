#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "camera.h"
#include "billboard.h"
#include "skybox.h"
#include "config_file.h"
#include "TerrainFromQuadtree.h"

using namespace std;

// je sais tjrs pas si ca sert
GLfloat ambientLight[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat diffuseLight[] = {
    1.0f,
    1.0f,
    1.0f,
    1.0f,
};

GLfloat matAmbient[] = {
    1.0f,
    1.0f,
    1.0f,
    1.0f,
};

Config config;
SDL_Event event;
SDL_Window *window;
SDL_Renderer *renderer;
camera cam;
TerrainFromQuadtree *terrain;
bool isRunning;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void setupAmbientLight()
{

  glEnable(GL_LIGHTING);

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

  glEnable(GL_LIGHT0);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
}

void checkKeyPress()
{
  SDL_PumpEvents();
  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
      isRunning = false;

    if (event.type == SDL_KEYDOWN)
    {
      //on quitte
      if (event.key.keysym.sym == SDLK_ESCAPE)
        isRunning = false;

      //mode wireframe
      if (event.key.keysym.sym == SDLK_w)
      {
        terrain->setWireframe();
      }
      //on peut sortir la souris de l´app sans quitter
      if (event.key.keysym.sym == SDLK_p)
      {
        cam.mouseIn(false);
      }
      // on augmente la distance du LOD
      if (event.key.keysym.sym == SDLK_m)
      {
        terrain->setLOD_range(10);
      }
      //on diminue la distance du LOD
      if (event.key.keysym.sym == SDLK_l)
      {
        terrain->setLOD_range(-10);
      }
    }
  }
  //on rentre dans l´app
  if (event.type == SDL_MOUSEBUTTONDOWN)
    cam.mouseIn(true);
}

//init avec des fonctions un peu random pas tres sur de leur utilité
void initOpenGL()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  setupAmbientLight();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
}

//on set le viewport de notre window
int setViewport(int width, int height)
{
  GLfloat AspectRatio;

  if (height == 0)
  {
    height = 1;
  }

  AspectRatio = (GLfloat)width / (GLfloat)height;

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  gluPerspective(config.field_of_view, AspectRatio, config.znear, config.zfar);

  glMatrixMode(GL_MODELVIEW);
  initskybox();

  glEnable(GL_DEPTH_TEST);

  return 1;
}

//main classique
int main(int argc, char **argv)
{
  loadConfig(config);

  cout << "*********************** Checkup SDL ***********************" << endl;
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    const char *error = SDL_GetError();
    fprintf(
        stderr,
        "Erreur lors de l'intialisation de la SDL : %s\n", error);

    SDL_Quit();
    return EXIT_FAILURE;
  }
  cout << "*********************** Creation Terrain ***********************" << endl;

  float Billboardscale = 10;
  Vect3f BillboardScaleVect = {Billboardscale, Billboardscale, Billboardscale};
  terrain = new TerrainFromQuadtree(config.heightmap_file, config.xsize, config.ysize, config.terrainHeightScale, config.terrain_Scale, config.terrain_Precision);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  cout << "*************** Initialisation SDL *************" << endl;
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_CreateWindowAndRenderer(config.WINDOW_WIDTH, config.WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer);

  SDL_SetWindowTitle(window, "VisuTerreImac");

  initOpenGL();

  isRunning = true;
  setViewport(config.WINDOW_WIDTH, config.WINDOW_HEIGHT);
  Uint32 startTime = SDL_GetTicks();
  while (isRunning)
  {

    checkKeyPress();

    startTime = SDL_GetTicks();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    cam.UpdateRotation();

    glLoadMatrixf(glm::value_ptr(cam.matrice4));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam.Control(window);

    drawSkybox(config.xsize * config.terrain_Scale);
    cam.UpdateTranslation();
    glLoadMatrixf(glm::value_ptr(cam.matrice4));

    glPushMatrix();
    terrain->render(cam);
    
    glPopMatrix();

    glPushMatrix();
    Vect3f BillboardPos = {10, terrain->getTerrainHeight({10, 0., 10}) + Billboardscale, 10};
    glTranslatef(BillboardPos.x, BillboardPos.y, BillboardPos.z);
    addBillboard(BillboardScaleVect);
    glPopMatrix();

    glPushMatrix();
    Vect3f BillboardPos1 = {50, terrain->getTerrainHeight({50, 0., 50}) + Billboardscale, 50};
    glTranslatef(BillboardPos1.x, BillboardPos1.y, BillboardPos1.z);
    addBillboard(BillboardScaleVect);
    glPopMatrix();

    glPushMatrix();
    Vect3f BillboardPos2 = {120, terrain->getTerrainHeight({120, 0., 120}) + Billboardscale, 120};
    glTranslatef(BillboardPos2.x, BillboardPos2.y, BillboardPos2.z);
    addBillboard(BillboardScaleVect);
    glPopMatrix();

    SDL_GL_SwapWindow(window);
    //cout << terrain->getTerrainHeight({10,0,10}) << endl;

    /* Calcul du temps ecoule */
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    /* Si trop peu de temps s'est ecoule, on met en pause le programme */
    if (elapsedTime < FRAMERATE_MILLISECONDS)
    {
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
    }
  }

  // on delete les objets pour liberer la memoire
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  delete terrain;
  SDL_DestroyWindow(window);
  renderer = NULL;
  window = NULL;
  killskybox();
  SDL_Quit();

  return EXIT_SUCCESS;
}
