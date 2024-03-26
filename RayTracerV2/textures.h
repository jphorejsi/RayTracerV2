#pragma once
#include <string>
#include "vec.h"
struct SceneType;

struct TextureType {
	int width, height, maxValue;
	Vec3** textureArray;
	TextureType(int widthI = 0, int heightI = 0, int maxValueI = 0) : width(widthI), height(heightI), maxValue(maxValueI) {
		this->textureArray = new Vec3 * [width];
		for (int i = 0; i < width; i++) {
			this->textureArray[i] = new Vec3[height];
		}
	}
};

bool isTextured(const SceneType& scene, const std::string objectType, const int objectNumber);
const TextureType& getTexture(const SceneType& scene, const std::string objectType, const int objectNumber);
Vec2 getTextureCoordinate(const SceneType& scene, const std::string objectType, const int& objectNumber, Vec3& intersectionPoint);
//Vec2 textureCoordinate(const SceneType& scene, const Vec3& intersectionPoint) const;
Vec3 getColor(const SceneType& scene, const std::string objectType, const int objectNumber, Vec3& intersectionPoint);
Vec3 getNormal(const SceneType& scene, const std::string objectType, const int objectNumber, Vec3& intersectionPoint);