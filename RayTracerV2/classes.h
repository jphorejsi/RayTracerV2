#pragma once
#ifndef CLASSES_H
#define CLASSES_H

struct Vec3 {
	float x, y, z;
	Vec3 operator+(const Vec3& other) const; //operator overloads
	Vec3 operator-(const Vec3& other) const;
	Vec3 operator*(float scalar) const; //scalar
	Vec3 operator-() const;
	Vec3 operator/(float scalar) const;
	Vec3(float xi = 1.0, float yi = 2.0, float zi = 3.0) : x(xi), y(yi), z(zi) { //default constructor
	}
};

struct ColorType {
	float r, g, b;
};

struct FovType { //type for storing vertical and horizontal fov
	float vertical, horizontal;
	FovType(float verticalI = 0, float horizontalI = 0) : vertical(verticalI), horizontal(horizontalI) { //default constructor
	}
};

struct ISizeType { //type for storing image size properties
	int width, height;
	ISizeType(int widthI = 0, int heightI = 0) : width(widthI), height(heightI) { //default constructor
	}
};


struct ViewingWindowType {
	//FSizeType size;
	float width, height;
	Vec3 ul, ur, ll, lr;
};

struct RayType { // Type for rays
	Vec3 position, direction;
	RayType(Vec3 positionI = Vec3(), Vec3 directionI = Vec3()) : position(positionI), direction(directionI) {
	}
};

typedef struct LightType { //type for storing light properties
	Vec3 position, color;
	int type; // type of the light, 0 for point light source, 1 for directional light source
	LightType(Vec3 pos = Vec3(), int t = 0, Vec3 col = Vec3()) : position(pos), type(t), color(col) { // default constructor
	}

};

typedef struct AttLightType : LightType { // inherit from the light class 
	float c1, c2, c3;
	AttLightType(Vec3 pos = Vec3(), int t = 0, Vec3 col = Vec3(), float c1I = 0, float c2I = 0, float c3I = 0) : LightType(pos, t, col), c1(c1I), c2(c2I), c3(c3I) { // default constructor
	}
};

struct MaterialType { //type for storing material properties
	Vec3 od, os;
	float ka, kd, ks;
	int n;
	MaterialType(Vec3 odI = Vec3(), Vec3 osI = Vec3(), float kaI = 0, float kdI = 0, float ksI = 0, int nI = 0) : od(odI), os(osI), ka(kaI), kd(kdI), ks(ksI), n(nI) { //default constructor
	}
};

struct SphereType {
	float radius;
	Vec3 position;
	MaterialType m;
	SphereType(Vec3 pos = Vec3(), float rad = 0, MaterialType mI = MaterialType()) : position(pos), radius(rad), m(mI) {
	}
};

struct DepthCueType {
	Vec3 dc;
	float alphaMax, alphaMin, distanceMax, distanceMin;
	bool enabled = false;
	DepthCueType(Vec3 dcI = Vec3(), float aMaxI = 0.0, float aMinI = 0.0, float distMaxI = 0.0, float distMinI = 0.0) : dc(dcI), alphaMax(aMaxI), alphaMin(aMinI), distanceMax(distMaxI), distanceMin(distMinI) {
	}
};

struct SceneType {
	Vec3 eyePosition, upDirection, viewDirection, backgroundColor;
	FovType fov;
	ISizeType imageSize;
	ViewingWindowType viewingWindow;
	LightType lights[50];
	int lightCount = 0;
	SphereType spheres[50];
	int sphereCount = 0;
	AttLightType attLights[50];
	int attLightCount = 0;
	DepthCueType depthCue;
};


#endif CLASSES_H