#ifndef VECT3F_h
#define VECT3F_h

#include <iostream>
#include <cmath>
#include "glm/glm.hpp"

class Vect3f{
	public:
		float x,y,z;
		Vect3f();
		Vect3f(glm::vec3 vec3);
		Vect3f(float a,float b);
		Vect3f(float a,float b,float c);
		
		float dotProduct(const Vect3f& vec2);
		Vect3f crossProduct(const Vect3f& vec2);
		float length();
		void normalize();
		
		void change(float a,float b,float c);
		void change(Vect3f vec2);
		void changeX(float a);
		void changeY(float a);
		void changeZ(float a);

		operator glm::vec3();
		
		Vect3f operator+(const Vect3f& vec2);
		Vect3f operator-(const Vect3f& vec2);
		Vect3f operator*(float num);
		Vect3f operator/(float num);
		
		Vect3f& operator+=(const Vect3f& vec2);
		Vect3f& operator-=(const Vect3f& vec2);
		Vect3f& operator*=(float num);
		Vect3f& operator/=(float num);	
		
		bool operator==(const Vect3f vec2);
		bool operator!=(const Vect3f vec2);
		
		friend std::ostream& operator<<(std::ostream& out,const Vect3f& vec);
};

#endif