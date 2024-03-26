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
	float ka, kd, ks;
	int n;
	MaterialType(Vec3 odI = Vec3(), Vec3 osI = Vec3(), float kaI = 0, float kdI = 0, float ksI = 0, int nI = 0) : od(odI), os(osI), ka(kaI), kd(kdI), ks(ksI), n(nI) {
	}
};

int shadowStatus(const SceneType &scene, const RayType &ray, const LightType &light, const int excliusion);
Vec3 lightShade(const SceneType &scene, const RayType &ray, Vec3 &intersectionPoint, const LightType &light, const std::string objectType, const int objectNumber);
Vec3 shadeRay(SceneType &scene, const std::string objectType, const int objectNumber, Vec3 &intersectionPoint, const RayType &ray);
float lightAttenuation(const Vec3 &intersectionPoint, const AttLightType &attLight);
const MaterialType& getMaterial(const SceneType& scene, const std::string objectType, const int objectNumber);