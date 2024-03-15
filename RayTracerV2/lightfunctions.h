#pragma once
#ifndef LIGHTFUNCTIONS_H
#define LIGHTFUNCTIONS_H

#include "scene.h"

int shadowStatus(SceneType scene, RayType ray, LightType light, int excliusion);
Vec3 lightShade(SceneType scene, RayType ray, Vec3 intersectionPoint, LightType light, std::string objectType, int objectNumber);
Vec3 shadeRay(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint, RayType ray);
float lightAttenuation(Vec3 intersectionPoint, AttLightType attLight);

#endif //LIGHTFUNCTIONS_H