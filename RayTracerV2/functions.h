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

//bool isNumber(const std::string& s);
const MaterialType &getMaterial(const SceneType &scene, const std::string objectType, const int objectNumber);	//done
std::tuple<std::string, int, float> intersectionCheck(const SceneType &scene, const RayType &ray, const int exclusion);	//done
float distance(const Vec3 &point1, const Vec3 &point2);	//done
float depthCueing(const SceneType &scene, const DepthCueType &depthCue, const Vec3 &intersectionPoint, const Vec3 &eye); //doone

#endif //FUNCTIONS_H