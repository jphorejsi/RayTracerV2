#include "sphere.h"

Vec3 SphereType::normal(Vec3 &intersectionPoint) {
	return Vec3((intersectionPoint.x - this->position.x) / this->radius, (intersectionPoint.y - this->position.y) / this->radius, (intersectionPoint.z - this->position.z) / this->radius);
}

Vec2 SphereType::textureCoordinate(Vec3 &intersectionPoint) {
	Vec3 n = this->normal(intersectionPoint);
	float u = 0.5 + std::atan2(n.y, n.x) / (2 * M_PI);
	float v = std::acos(n.z) / M_PI;
	return Vec2(u, v);
}