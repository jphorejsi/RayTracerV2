#include "triangle.h"
#include "functions.h"

Vec3 TriangleType::barycentric(SceneType scene, Vec3 intersectionPoint) {
	Vec3 p0 = scene.vertices[this->v0Index - 1];
	Vec3 p1 = scene.vertices[this->v1Index - 1];
	Vec3 p2 = scene.vertices[this->v2Index - 1];
	Vec3 e1 = p1 - p0;
	Vec3 e2 = p2 - p0;
	Vec3 ep = intersectionPoint - p0;
	float d11 = dot(e1, e1);
	float d12 = dot(e1, e2);
	float d22 = dot(e2, e2);
	float dp1 = dot(ep, e1);
	float dp2 = dot(ep, e2);
	float D = d11 * d22 - d12 * d12;
	float D_beta = d22 * dp1 - d12 * dp2;
	float D_gamma = d11 * dp2 - d12 * dp1;
	float beta = D_beta / D;
	float gamma = D_gamma / D;
	float alpha = 1 - beta - gamma;
	return Vec3(alpha, beta, gamma);
}

Vec3 TriangleType::normal(SceneType scene, Vec3 intersectionPoint) {
	if (this->smoothShaded) {
		Vec3 p0 = scene.vertices[this->v0Index - 1];
		Vec3 p1 = scene.vertices[this->v1Index - 1];
		Vec3 p2 = scene.vertices[this->v2Index - 1];
		Vec3 e1 = p1 - p0;
		Vec3 e2 = p2 - p0;
		return normalize(cross(e1, e2));
	}
	else {
		Vec3 bayrcentric_coordinates = this->barycentric(scene, intersectionPoint);
		Vec3 vn0 = scene.normalVerticies[this->vn0Index - 1];
		Vec3 vn1 = scene.normalVerticies[this->vn1Index - 1];
		Vec3 vn2 = scene.normalVerticies[this->vn2Index - 1];
		return normalize(vn0 * bayrcentric_coordinates.x + vn1 * bayrcentric_coordinates.y + vn2 * bayrcentric_coordinates.z);
	}
}

Vec2 TriangleType::textureCoordinate(SceneType scene, Vec3 intersectionPoint) {
	Vec3 barycentric_coordinate = this->barycentric(scene, intersectionPoint);
	Vec2 vt0 = scene.textureCordinates[this->vt0Index - 1];
	Vec2 vt1 = scene.textureCordinates[this->vt1Index - 1];
	Vec2 vt2 = scene.textureCordinates[this->vt2Index - 1];
	float u = barycentric_coordinate.x * vt0.x + barycentric_coordinate.y * vt1.x + barycentric_coordinate.z * vt2.x;
	float v = barycentric_coordinate.x * vt0.y + barycentric_coordinate.y * vt1.y + barycentric_coordinate.z * vt2.y;
	return Vec2(u, v);
}