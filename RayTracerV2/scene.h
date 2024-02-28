#pragma once
#ifndef SCENE_H
#define SCENE_H

//#include "classes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "classes.h"
#include "sphere.h"
#include "triangle.h"

struct TriangleType;

struct SceneType {
	Vec3 eyePosition, upDirection, viewDirection, backgroundColor;
	FovType fov;
	ISizeType imageSize;
	ViewingWindowType viewingWindow;
	std::vector<LightType> lights;
	std::vector<SphereType> spheres;
	std::vector<MaterialType> materials;
	std::vector<TextureType> textures;
	std::vector<AttLightType> attLights;
	std::vector<TriangleType> triangles;
	std::vector<Vec3> vertices;
	std::vector<Vec3> normalVerticies;
	std::vector<Vec2> textureCordinates;
	DepthCueType depthCue;
};

#endif //SCENE_H