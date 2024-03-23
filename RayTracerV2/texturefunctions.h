#pragma once
#ifndef TEXTUREFUNCTIONS_H
#define TEXTUREFUNCTIONS_H

#include "scene.h"

bool isNormalMapped(const SceneType &scene, const std::string objectType, const int objectNumber);
const BumpType &getNormalMap(const SceneType &scene, const std::string objectType, const int objectNumber);
Vec3 normalMapping(const SceneType &scene, const std::string objectType, const int objectNumber, Vec3 &intersectionPoint);
Vec3 getColor(const SceneType &scene, const std::string objectType, const int objectNumber, Vec3 &intersectionPoint);
Vec3 getNormal(const SceneType &scene, const std::string objectType, const int objectNumber, Vec3 &intersectionPoint);
Vec2 getTextureCoordinate(const SceneType &scene, const std::string objectType, const int& objectNumber, Vec3& intersectionPoint);
const TextureType &getTexture(const SceneType &scene, const std::string objectType, const int objectNumber);
bool isTextured(const SceneType &scene, const std::string objectType, const int objectNumber);

#endif //TEXTUREFUNCTIONS_H