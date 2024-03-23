#include "sphere.h"

Vec3 SphereType::normal(Vec3 &intersectionPoint) const{
	return Vec3((intersectionPoint.x - this->position.x) / this->radius, (intersectionPoint.y - this->position.y) / this->radius, (intersectionPoint.z - this->position.z) / this->radius);
}

Vec2 SphereType::textureCoordinate(Vec3 &intersectionPoint) const{
	Vec3 n = this->normal(intersectionPoint);
	float u = 0.5 + std::atan2(n.y, n.x) / (2 * 3.14159265358979323846);
	float v = std::acos(n.z) / 3.14159265358979323846;
	return Vec2(u, v);
}