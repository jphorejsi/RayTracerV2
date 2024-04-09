#include "bump.h"
#include "textures.h"
#include "triangle.h"
#include "scene.h"

bool isNormalMapped(const SceneType& scene, const std::string objectType, const int objectNumber) {
    if (objectType == "Sphere") {
        return scene.spheres[objectNumber].bumpId != -1;
    }
    else return scene.triangles[objectNumber].bumpId != -1;
}

const BumpType& getNormalMap(const SceneType& scene, const std::string objectType, const int objectNumber) {
    if (objectType == "Sphere") {
        return scene.bumps[scene.spheres[objectNumber].bumpId];
    }
    else return scene.bumps[scene.triangles[objectNumber].bumpId];
}

Vec3 normalMapping(const SceneType& scene, const std::string objectType, const int objectNumber, Vec3& intersectionPoint) {
    Vec2 TC = getTextureCoordinate(scene, objectType, objectNumber, intersectionPoint);
    TextureType texture = getTexture(scene, objectType, objectNumber);
    int x = TC.x * (texture.width - 1); //i
    int y = TC.y * (texture.height - 1); //j
    Vec3 m = getNormalMap(scene, objectType, objectNumber).bumpArray[x][y];
    if (objectType == "Sphere") {
        Vec3 N = getNormal(scene, objectType, objectNumber, intersectionPoint);
        Vec3 T = Vec3(-N.y / sqrt(N.x * N.x + N.y * N.y), N.x / sqrt(N.x * N.x + N.y + N.y), 0.0);
        Vec3 B = N.cross(T);
        float nx = T.x * m.x + B.x * m.y + N.x * m.z;
        float ny = T.y * m.x + B.y * m.y + N.y * m.z;
        float nz = T.z * m.x + B.z * m.y + N.z * m.z;
        return Vec3(nx, ny, nz);
    }
    else {
        Vec3 N = getNormal(scene, objectType, objectNumber, intersectionPoint);
        TriangleType triangle = scene.triangles[objectNumber];
        Vec3 p0 = scene.vertices[triangle.v0Index - 1];
        Vec3 p1 = scene.vertices[triangle.v1Index - 1];
        Vec3 p2 = scene.vertices[triangle.v2Index - 1];
        Vec2 TC0 = scene.textureCordinates[triangle.vt0Index - 1];
        Vec2 TC1 = scene.textureCordinates[triangle.vt1Index - 1];
        Vec2 TC2 = scene.textureCordinates[triangle.vt2Index - 1];
        float u1 = TC1.x - TC0.x;
        float v1 = TC1.y - TC0.y;
        float u2 = TC2.x - TC1.x;
        float v2 = TC2.y - TC1.y;
        Vec3 e1 = p1 - p0;
        Vec3 e2 = p2 - p1;
        float invDet = 1.0f / (u1 * v2 - u2 * v1);
        Vec3 T = (e1 * v2 - e2 * v1) * invDet;
        Vec3 B = (e2 * u1 - e1 * u2) * invDet;
        T = T.normal();
        B = B.normal();
        Vec3 newN = (T * m.x + B * m.y + N * m.z).normal();
        return newN;
    }
}