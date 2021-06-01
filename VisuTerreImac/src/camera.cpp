#include "camera.h"
using namespace std;

Config configCamera;

//on verrifie qu´on fait pas des loopings
void camera::lockCamera()
{
	if (cam_pitch > 90)
		cam_pitch = 90;
	if (cam_pitch < -90)
		cam_pitch = -90;
	if (cam_yaw < 0.0)
		cam_yaw += 360.0;
	if (cam_yaw > 360.0)
		cam_yaw -= 360;
}


Vect3f camera::getPosition()
{
	return Vect3f(cam_coord.x, cam_coord.y, cam_coord.z);
}


void camera::moveCamera(float dir)
{
	float rad = (cam_yaw + dir) * M_PI / 180.0;
	cam_coord.x -= sin(rad) * move_velocity;
	cam_coord.z -= cos(rad) * move_velocity;
}

void camera::moveCameraUp(float dir)
{
	float rad = (cam_pitch + dir) * M_PI / 180.0;
	cam_coord.y += sin(rad) * move_velocity;
}

void camera::moveCameraUpStatic()
{
	cam_coord.y++;
}

void camera::moveCameraDownStatic()
{
	cam_coord.y--;
}

camera::camera()
{
	
	cam_pitch = 0;
	cam_yaw = 0;
	move_velocity = 2.5;
	mouse_velocity = 0.05;
	mouse_in = ismoved = false;
	matrice4 = glm::mat4(1);
}

void camera::Control(SDL_Window *window)
{
loadConfig(configCamera);
	if (mouse_in)
	{
		int MidX = configCamera.WINDOW_WIDTH / 2;
		int MidY = configCamera.WINDOW_HEIGHT / 2;
		int tmpx, tmpy;
		SDL_ShowCursor(SDL_DISABLE);
		SDL_GetMouseState(&tmpx, &tmpy);
		cam_yaw += mouse_velocity * (MidX - tmpx);
		cam_pitch += mouse_velocity * (MidY - tmpy);
		lockCamera();
		SDL_WarpMouseInWindow(window, MidX, MidY);
		ismoved = false;
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		float coordX = cam_coord.x;
		float coordY = cam_coord.y;
		SDL_PumpEvents();
		if (state[SDL_SCANCODE_W])
		{
			ismoved = true;
			if (cam_pitch != 90 && cam_pitch != -90)
				moveCamera(0.0);
			moveCameraUp(0.0);
		}
		else if (state[SDL_SCANCODE_S])
		{
			ismoved = true;
			if (cam_pitch != 90 && cam_pitch != -90)
				moveCamera(180.0);
			moveCameraUp(180.0);
		}
		else if (state[SDL_SCANCODE_UP])
		{
			ismoved = true;
			moveCameraUpStatic();
		}
		else if (state[SDL_SCANCODE_DOWN])
		{
			ismoved = true;
			moveCameraDownStatic();
		}
		if (state[SDL_SCANCODE_A])
		{
			ismoved = true;
			moveCamera(90.0);
		}
		else if (state[SDL_SCANCODE_D])
		{
			ismoved = true;
			moveCamera(270);
		}
	}
}

void camera::UpdateRotation()
{
	matrice4 = glm::rotate(glm::radians(-cam_pitch), glm::vec3(1, 0, 0));
	matrice4 *= glm::rotate(glm::radians(-cam_yaw), glm::vec3(0, 1, 0));
}

void camera::UpdateTranslation()
{
	matrice4 *= glm::translate(glm::vec3(-cam_coord.x, -cam_coord.y, -cam_coord.z));
}

void camera::mouseIn(bool b)
{
	mouse_in = b;
}

bool camera::isMoved()
{
	return ismoved;
}

void camera::lookAt(float pitch, float yaw)
{
	cam_pitch = pitch;
	cam_yaw = yaw;
}

glm::vec3 camera::getVector()
{
	return (-glm::normalize(glm::vec3(glm::row(matrice4, 2))));
}

IntersectionResult camera::IntersectRayPlane(Vect3f origin, Vect3f normale)
{
	Vect3f rayOrigin = cam_coord;
	Vect3f direction = getVector();
	float t = ((origin - rayOrigin).dotProduct(normale)) / direction.dotProduct(normale);
	IntersectionResult res;
	if (t > 0)
	{
		res.pt = rayOrigin + direction * t;
		res.succeeded = true;
	}

	return res;
}

IntersectionResult camera::IntersectRayTriangle(Vect3f p1, Vect3f p2, Vect3f p3)
{
	Vect3f normale = (p2 - p1).crossProduct(p3 - p1);
	normale.normalize();
	Vect3f interPlane = IntersectRayPlane(p1, normale).pt;
	glm::mat3 matrice = glm::mat3(p2 - p1, p3 - p1, normale);
	matrice = glm::inverse(matrice);
	glm::vec3 v = matrice * (interPlane - p1);
	IntersectionResult res;
	res.succeeded = v.x + v.y <= 1.f && v.x >= 0 && v.y >= 0;
	res.pt = interPlane;
	return res;
}
