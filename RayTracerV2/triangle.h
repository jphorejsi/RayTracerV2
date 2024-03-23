#pragma once
#ifndef TRIANGLES_H
#define TRIANGLES_H

#include "classes.h"
#include "scene.h"

struct SceneType;

struct TriangleType {
	int materialId, textureId, bumpId, Id = -1;
	int v0Index, v1Index, v2Index;
	int vn0Index, vn1Index, vn2Index;
	int vt0Index, vt1Index, vt2Index;
	bool smoothShaded, textureMapped;

	TriangleType(int materialIdI = -1, int textureIdI = -1, int bumpIdI = -1, int v0IndexI = -1, int v1IndexI = -1, int v2IndexI = -1, bool smoothShadedI = false, int vn0IndexI = -1, int vn1IndexI = -1, int vn2IndexI = -1, bool textureMappedI = false, int vt0IndexI = -1, int vt1IndexI = -1, int vt2IndexI = -1) :
		materialId(materialIdI), textureId(textureIdI), bumpId(bumpIdI), v0Index(v0IndexI), v1Index(v1IndexI), v2Index(v2IndexI), smoothShaded(smoothShadedI), vn0Index(vn0IndexI), vn1Index(vn1IndexI), vn2Index(vn2IndexI), textureMapped(textureMappedI), vt0Index(vt0IndexI), vt1Index(vt1IndexI), vt2Index(vt2IndexI) {}

	Vec3 barycentric(const SceneType &scene,const Vec3 &intersectionPoint) const;
	Vec3 normal(const SceneType &scene, const Vec3 &intersectionPoint) const;
	Vec2 textureCoordinate(const SceneType &scene, const Vec3 &intersectionPoint) const;
};

#endif //TRIANGLES_H
