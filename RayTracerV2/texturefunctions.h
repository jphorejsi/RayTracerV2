#pragma once
#ifndef TEXTUREFUNCTIONS_H
#define TEXTUREFUNCTIONS_H

#include "scene.h"

bool isNormalMapped(SceneType scene, std::string objectType, int objectNumber);
BumpType getNormalMap(SceneType scene, std::string objectType, int objectNumber);
Vec3 normalMapping(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint);
Vec3 getColor(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint);
Vec3 getNormal(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint);
Vec2 getTextureCoordinate(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint);
TextureType getTexture(SceneType scene, std::string objectType, int objectNumber);
bool isTextured(SceneType scene, std::string objectType, int objectNumber);

#endif //TEXTUREFUNCTIONS_H