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
bool isTextured(SceneType scene, std::string objectType, int objectNumber);
MaterialType getMaterial(SceneType scene, std::string objectType, int objectNumber);
Vec2 getTextureCoordinate(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint);
TextureType getTexture(SceneType scene, std::string objectType, int objectNumber);
Vec3 getOd(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint);

#endif //FUNCTIONS_H