#pragma once
#ifndef TRIANGLES_H
#define TRIANGLES_H

#include "classes.h"
#include "scene.h"

struct SceneType;

struct TriangleType {
	int materialId, textureId, bumpId;

	int v0Index, v1Index, v2Index;
	int vn0Index, vn1Index, vn2Index;
	int vt0Index, vt1Index, vt2Index;

	bool smoothShaded, textureMapped;

	Vec3 barycentric(SceneType scene, Vec3 intersectionPoint);
	Vec3 normal(SceneType scene, Vec3 intersectionPoint);
	Vec2 textureCoordinate(SceneType scene, Vec3 intersectionPoint);
};

#endif //TRIANGLES_H
