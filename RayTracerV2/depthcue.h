#pragma once
#include "vec.h"
struct SceneType;

struct DepthCueType {
	Vec3 dc;
	float alphaMax, alphaMin, distanceMax, distanceMin;
	bool enabled = false;
	DepthCueType(Vec3 dcI = Vec3(), float aMaxI = 0.0, float aMinI = 0.0, float distMaxI = 0.0, float distMinI = 0.0) : dc(dcI), alphaMax(aMaxI), alphaMin(aMinI), distanceMax(distMaxI), distanceMin(distMinI) {
	}
};

float depthCueing(const SceneType& scene, const DepthCueType& depthCue, const Vec3& intersectionPoint, const Vec3& eye);