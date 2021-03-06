#include "Quat.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

// Methods
Quat::Quat(){}

Quat::Quat(const Quat& other){
	this->w = other.w;
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Quat::Quat(float w, float x, float y, float z){
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quat::Quat(const Vec3& angles){
	float a1 = cos(angles.y / 2.f);
	float a2 = cos(angles.z / 2.f);
	float a3 = cos(angles.x / 2.f);
	
	float b1 = sin(angles.y / 2.f);
	float b2 = sin(angles.z / 2.f);
	float b3 = sin(angles.x / 2.f);
	
	w = a1 * a2 * a3 - b1 * b2 * b3;
	x = -(b1 * b2 * a3 - a1 * a2 * b3);
	y = b1 * a2 * a3 + a1 * b2 * b3;
	z = a1 * b2 * a3 - b1 * a2 * b3;
}

void Quat::operator=(const Quat& other){
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

Quat Quat::operator*(const Quat& other){
	return Quat(
		other.w * w - other.y * y - other.z * z - other.x * x,
		other.w * x - other.y * z + other.z * y + other.x * w,
		other.w * y + other.y * w - other.z * x + other.x * z,
		other.w * z + other.y * x + other.z * w - other.x * y
	);
}

void Quat::operator*=(const Quat& other){
	(*this) = (*this) * other;
}

Vec3 Quat::operator*(const Vec3& other){
	Quat converted(0.f, other.x, other.y, other.z);

	converted = (*this) * converted * conjugate(*this);
	
	return Vec3(converted.x, converted.y, converted.z);
}

// Functions
Quat conjugate(const Quat& a){
	return Quat(a.w, -a.x, -a.y, -a.z);
}

Vec3 eulerAngles(const Quat& a){
	float x = atan2f(2 * (a.w * a.x + a.y * a.z), 1 - 2 * (pow(a.x, 2) + pow(a.y, 2)));

	float y = asin(2 * (a.w * a.y - a.z * a.x));

	float z = atan2(2 * (a.w * a.z + a.x * a.y), 1 - 2 * (pow(a.y, 2) + pow(a.z, 2)));

	return Vec3(x, y, z);
}

Quat inverse(const Quat& a){
	Quat c = conjugate(a);

	return c * c * Quat(a);
}

float magnitude(const Quat& a){
	return sqrt(pow(a.w, 2) + pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

Quat normalize(const Quat& a){
	float m = magnitude(a);

	return Quat(a.w / m, a.x / m, a.y / m, a.z / m);
}