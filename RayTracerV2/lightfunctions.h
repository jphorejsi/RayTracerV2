#pragma once
#ifndef LIGHTFUNCTIONS_H
#define LIGHTFUNCTIONS_H

#include "scene.h"

int shadowStatus(const SceneType &scene, const RayType &ray, const LightType &light, const int excliusion);
Vec3 lightShade(const SceneType &scene, const RayType &ray, Vec3 &intersectionPoint, const LightType &light, const std::string objectType, const int objectNumber);
Vec3 shadeRay(SceneType &scene, const std::string objectType, const int objectNumber, Vec3 &intersectionPoint, const RayType &ray);
float lightAttenuation(const Vec3 &intersectionPoint, const AttLightType &attLight);

#endif //LIGHTFUNCTIONS_H