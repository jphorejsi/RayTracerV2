#pragma once
#include <string>
#include "vec.h"
struct SceneType;

struct BumpType {
	int width, height, maxValue;
	Vec3** bumpArray;
	BumpType(int widthI = 0, int heightI = 0, int maxValueI = 255) : width(widthI), height(heightI), maxValue(maxValueI) {
		this->bumpArray = new Vec3 * [width];
		for (int i = 0; i < width; i++) {
			this->bumpArray[i] = new Vec3[height];
		}
	}
};

bool isNormalMapped(const SceneType& scene, const std::string objectType, const int objectNumber);
const BumpType& getNormalMap(const SceneType& scene, const std::string objectType, const int objectNumber);
Vec3 normalMapping(const SceneType& scene, const std::string objectType, const int objectNumber, Vec3& intersectionPoint);