#pragma once
#include <vector>
#include "sphere.h"
#include "lights.h"
#include "triangle.h"
#include "textures.h"
#include "bump.h"
#include "depthcue.h"


struct ViewingWindowType {
	float width, height;
	Vec3 ul, ur, ll, lr;
};

struct SceneType {
	Vec3 eyePosition, upDirection, viewDirection, backgroundColor;
	Vec2 fov, imageSize;
	ViewingWindowType viewingWindow;
	std::vector<LightType> lights;
	std::vector<SphereType> spheres;
	std::vector<MaterialType> materials;
	std::vector<TextureType> textures;
	std::vector<AttLightType> attLights;
	TriangleType* triangles;
	int trianglesSize = 0;
	Vec3* vertices;
	int verticesSize = 0;
	Vec3* normalVertices;
	int normalVerticesSize = 0;
	std::vector<Vec2> textureCordinates;
	std::vector<BumpType> bumps;
	DepthCueType depthCue;
};
