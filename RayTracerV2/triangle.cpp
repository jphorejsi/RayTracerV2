#include "triangle.h"
#include "functions.h"

Vec3 TriangleType::barycentric(const SceneType &scene, const Vec3 &intersectionPoint) const {
	Vec3 p0 = scene.vertices[this->v0Index - 1];
	Vec3 p1 = scene.vertices[this->v1Index - 1];
	Vec3 p2 = scene.vertices[this->v2Index - 1];
	Vec3 e1 = p1 - p0;
	Vec3 e2 = p2 - p0;
	Vec3 ep = intersectionPoint - p0;
	float d11 = e1.dot(e1);
	float d12 = e1.dot(e2);
	float d22 = e2.dot(e2);
	float dp1 = ep.dot(e1);
	float dp2 = ep.dot(e2);
	float D = d11 * d22 - d12 * d12;
	float D_beta = d22 * dp1 - d12 * dp2;
	float D_gamma = d11 * dp2 - d12 * dp1;
	float beta = D_beta / D;
	float gamma = D_gamma / D;
	float alpha = 1 - beta - gamma;
	return Vec3(alpha, beta, gamma);
}

Vec3 TriangleType::normal(const SceneType &scene,const Vec3 &intersectionPoint) const {
	if (!this->smoothShaded) {
		Vec3 p0 = scene.vertices[this->v0Index - 1];
		Vec3 p1 = scene.vertices[this->v1Index - 1];
		Vec3 p2 = scene.vertices[this->v2Index - 1];
		Vec3 e1 = p1 - p0;
		Vec3 e2 = p2 - p0;
		return e1.cross(e2).normal();
	}
	else {
		Vec3 bayrcentric_coordinates = this->barycentric(scene, intersectionPoint);
		Vec3 vn0 = scene.normalVerticies[this->vn0Index - 1];
		Vec3 vn1 = scene.normalVerticies[this->vn1Index - 1];
		Vec3 vn2 = scene.normalVerticies[this->vn2Index - 1];
		return (vn0 * bayrcentric_coordinates.x + vn1 * bayrcentric_coordinates.y + vn2 * bayrcentric_coordinates.z).normal();
	}
}

Vec2 TriangleType::textureCoordinate(const SceneType &scene, const Vec3 &intersectionPoint) const {
	Vec3 barycentric_coordinate = this->barycentric(scene, intersectionPoint);
	Vec2 vt0 = scene.textureCordinates[this->vt0Index - 1];
	Vec2 vt1 = scene.textureCordinates[this->vt1Index - 1];
	Vec2 vt2 = scene.textureCordinates[this->vt2Index - 1];
	float u = barycentric_coordinate.x * vt0.x + barycentric_coordinate.y * vt1.x + barycentric_coordinate.z * vt2.x;
	float v = barycentric_coordinate.x * vt0.y + barycentric_coordinate.y * vt1.y + barycentric_coordinate.z * vt2.y;
	return Vec2(u, v);
}