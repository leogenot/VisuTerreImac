#ifndef CAMERA_H
#define CAMERA_H
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Vect3f.h"
#include "config_file.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_access.hpp"

struct IntersectionResult
{
	bool succeeded;
	Vect3f pt;
};

class camera
{
	
	float cam_pitch, cam_yaw;
	float move_velocity;
	float mouse_velocity;
	bool mouse_in, ismoved;
	void lockCamera();
	void moveCamera(float dir);
	void moveCameraUp(float dir);
	void moveCameraUpStatic();
	void moveCameraDownStatic();

public:
	Vect3f cam_coord;
	glm::mat4 matrice4;
	camera();
	Vect3f getPosition();
	void Control(SDL_Window *window);
	void UpdateRotation();
	void UpdateTranslation();
	Vect3f getLocation();
	glm::vec3 getVector();
	void mouseIn(bool b);
	bool isMoved();
	void lookAt(float pitch, float yaw);
	IntersectionResult IntersectRayTriangle(Vect3f p1, Vect3f p2, Vect3f p3);
	IntersectionResult IntersectRayPlane(Vect3f origin, Vect3f normale);
};

#endif
