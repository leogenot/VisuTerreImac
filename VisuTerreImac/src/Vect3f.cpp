#include "Vect3f.h"
using namespace std;

Vect3f::Vect3f()
{
	x=y=z=0;
}

Vect3f::Vect3f(glm::vec3 vec3)
	: x(vec3.x), y(vec3.y), z(vec3.z)
{
}


Vect3f::Vect3f(float a,float b)
{
	x=a;
	y=b;
	z=0;
}

Vect3f::Vect3f(float a,float b,float c)
{
	x=a;
	y=b;
	z=c;
}
		
float Vect3f::dotProduct(const Vect3f& vec2)
{
	return (x*vec2.x+y*vec2.y+z*vec2.z);
}

Vect3f Vect3f::crossProduct(const Vect3f& vec2)
{
	return Vect3f(
		vec2.z * y - vec2.y * z,
		vec2.x * z - vec2.z * x,
		vec2.y * x - vec2.x * y 
	);
}




float Vect3f::length()
{
	return sqrt(x*x+y*y+z*z);
}

void Vect3f::normalize()
{
	float len=length();
	if(len!=0)
	{
		x/=len;
		y/=len;
		z/=len;
	}
}
		
void Vect3f::change(float a,float b,float c)
{
	x=a;
	y=b;
	z=c;
}

void Vect3f::change(Vect3f vec2)
{
	x=vec2.x;
	y=vec2.y;
	z=vec2.z;
}

void Vect3f::changeX(float a)
{
	x=a;
}
void Vect3f::changeY(float a)
{
	y=a;
}
void Vect3f::changeZ(float a)
{
	z=a;
}

Vect3f::operator glm::vec3() {
	return glm::vec3(x, y, z);
}
		
Vect3f Vect3f::operator+(const Vect3f& vec2)
{
	return Vect3f(x+vec2.x,y+vec2.y,z+vec2.z);
}

Vect3f Vect3f::operator-(const Vect3f& vec2)
{
	return Vect3f(x-vec2.x,y-vec2.y,z-vec2.z);
}

Vect3f Vect3f::operator*(float num)
{
	return Vect3f(x*num,y*num,z*num);
}

Vect3f Vect3f::operator/(float num)
{
	if(num!=0)
		return Vect3f(x/num,y/num,z/num);
	else
		return Vect3f();
}

Vect3f& Vect3f::operator+=(const Vect3f& vec2)
{
	x+=vec2.x;
	y+=vec2.y;
	z+=vec2.z;
	return *this;
}

Vect3f& Vect3f::operator-=(const Vect3f& vec2)
{
	x-=vec2.x;
	y-=vec2.y;
	z-=vec2.z;
	return *this;
}

Vect3f& Vect3f::operator*=(float num)
{
	x*=num;
	y*=num;
	z*=num;
	return *this;
}

Vect3f& Vect3f::operator/=(float num)	
{
	if(num!=0)
	{
		x/=num;
		y/=num;
		z/=num;
	}
	return *this;
}

bool Vect3f::operator==(const Vect3f vec2)
{
	return (x==vec2.x && y==vec2.y && z==vec2.z);
}

bool Vect3f::operator!=(const Vect3f vec2)
{
	!(*this==vec2);
}
		
ostream& operator<<(ostream& out,const Vect3f& vec)
{
	out << vec.x << " " << vec.y << " " << vec.z << endl;
	return out;
}