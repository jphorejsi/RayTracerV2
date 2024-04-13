#pragma once
#include <tuple>
#include <string>
#include "vec.h"
struct SceneType;

struct ColorType {
	float r, g, b;
};

struct RayType {
	Vec3 position, direction;
	RayType(Vec3 positionI = Vec3(), Vec3 directionI = Vec3()) : position(positionI), direction(directionI) {
	}
};

std::tuple<std::string, int, float> intersectionCheck(const SceneType& scene, const RayType& ray, const int exclusion);
std::tuple<std::string, int, float> intersectionCheck(const SceneType& scene, const RayType& ray);
float distance(const Vec3& point1, const Vec3& point2);
bool rayTriangleIntersection(const SceneType& scene, int id, const RayType& ray);
bool raySphereIntersection(const SceneType& scene, int id, const RayType& ray);