#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "classes.h"

struct SphereType {
	float radius;
	Vec3 position;
	int materialId, textureId, bumpId;
	SphereType(Vec3 pos = Vec3(), float rad = 0, int materialIdI = -1, int textureIdI = -1, int bumpIdI = -1) : position(pos), radius(rad), materialId(materialIdI), textureId(textureIdI), bumpId(bumpIdI) {
	}
	Vec3 normal(Vec3 &intersectionPoint);
	Vec2 textureCoordinate(Vec3 &intersectionPoint);
};

#endif //SPHERE_H