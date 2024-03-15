#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//#include "scene.h"

#include <string>
#include <tuple>
#include "classes.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"

bool isNumber(const std::string& s);
Vec3 cross(Vec3 u, Vec3 v);
float magnitude(const Vec3& v);
Vec3 normalize(Vec3 v);
float dot(Vec3 u, Vec3 v);
float distance(Vec3 u, Vec3 v);
MaterialType getMaterial(SceneType scene, std::string objectType, int objectNumber);
std::tuple<std::string, int, float> intersectionCheck(SceneType scene, RayType ray, int exclusion);
float depthCueing(SceneType scene, DepthCueType depthCue, Vec3 intersectionPoint, Vec3 eye);

#endif //FUNCTIONS_H