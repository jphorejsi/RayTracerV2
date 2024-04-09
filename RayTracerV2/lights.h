#pragma once
#include "vec.h"
#include "ray.h"

struct LightType {
	Vec3 position, color;
	int type;
	LightType(Vec3 pos = Vec3(), int t = 0, Vec3 col = Vec3()) : position(pos), type(t), color(col) {
	}

};
struct AttLightType : LightType {
	float c1, c2, c3;
	AttLightType(Vec3 pos = Vec3(), int t = 0, Vec3 col = Vec3(), float c1I = 0, float c2I = 0, float c3I = 0) : LightType(pos, t, col), c1(c1I), c2(c2I), c3(c3I) {
	}
};

struct MaterialType {
	Vec3 od, os;
	float ka, kd, ks, n, alpha, eta;
	MaterialType(Vec3 odI = Vec3(), Vec3 osI = Vec3(), float kaI = 0, float kdI = 0, float ksI = 0, float nI = 0, float alphaI = 1, float etaI = 1) : od(odI), os(osI), ka(kaI), kd(kdI), ks(ksI), n(nI), alpha(alphaI), eta(etaI) { // might be changed
	}
};

struct DepthCueType {
	Vec3 dc;
	float alphaMax, alphaMin, distanceMax, distanceMin;
	bool enabled = false;
	DepthCueType(Vec3 dcI = Vec3(), float aMaxI = 0.0, float aMinI = 0.0, float distMaxI = 0.0, float distMinI = 0.0) : dc(dcI), alphaMax(aMaxI), alphaMin(aMinI), distanceMax(distMaxI), distanceMin(distMinI) {
	}
};

int shadowStatus(const SceneType &scene, const RayType &ray, const LightType &light, const int excliusion);
Vec3 lightShade(const SceneType &scene, const RayType &ray, Vec3 &intersectionPoint, const LightType &light, const std::string objectType, const int objectNumber);
Vec3 shadeRay(SceneType &scene, const std::string objectType, const int objectNumber, Vec3 &intersectionPoint, const RayType &ray);
float lightAttenuation(const Vec3 &intersectionPoint, const AttLightType &attLight);
const MaterialType& getMaterial(const SceneType& scene, const std::string objectType, const int objectNumber);
float depthCueing(const SceneType& scene, const DepthCueType& depthCue, const Vec3& intersectionPoint, const Vec3& eye);