//#include "functions.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <tuple>
#include "functions.h"
#include "classes.h"

bool isNumber(const std::string& s) { //determines if a string is a number.
    for (char const& ch : s) {
        if (std::isdigit(ch) == 0 && ch != '-' && ch != '.') {
            return false;
        }
    }
    return true;
}

Vec3 cross(Vec3 u, Vec3 v) {
    Vec3 result;
    result.x = u.y * v.z - u.z * v.y;
    result.y = u.z * v.x - u.x * v.z;
    result.z = u.x * v.y - u.y * v.x;
    return result;
}

float magnitude(const Vec3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 normalize(Vec3 v) {
    float mag = magnitude(v);
    if (mag == 0.0f) {
        // Avoid division by zero
        std::cerr << "Cannot normalize a zero vector." << std::endl;
        return { 0.0f, 0.0f, 0.0f };
    }
    Vec3 result;
    result.x = v.x / mag;
    result.y = v.y / mag;
    result.z = v.z / mag;
    return result;
}


float dot(Vec3 u, Vec3 v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

float distance(Vec3 u, Vec3 v) {
    return sqrt(std::pow(u.x - v.x, 2) + std::pow(u.y - v.y, 2) + std::pow(u.z - v.z, 2));
}

bool isTextured(SceneType scene, std::string objectType, int objectNumber) {
    if (objectType == "Sphere") {
        return (scene.spheres[objectNumber].textureId != -1);
    }
    else return (scene.triangles[objectNumber].textureId != 1);
}

MaterialType getMaterial(SceneType scene, std::string objectType, int objectNumber) {
    if (objectType == "Sphere") {
        return scene.materials[scene.spheres[objectNumber].materialId];
    }
    else return scene.materials[scene.triangles[objectNumber].materialId];
}

Vec2 getTextureCoordinate(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint) {
    if (objectType == "Sphere") {
        return scene.spheres[objectNumber].textureCoordinate(intersectionPoint);
    }
    else return scene.triangles[objectNumber].textureCoordinate(scene, intersectionPoint);
}

TextureType getTexture(SceneType scene, std::string objectType, int objectNumber) {
    if (objectType == "Sphere") {
        return scene.textures[scene.spheres[objectNumber].textureId];
    }
    else return scene.textures[scene.triangles[objectNumber].textureId];
}

Vec3 getOd(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint) {
    Vec2 TC = getTextureCoordinate(scene, objectType, objectNumber, intersectionPoint);
    TextureType texture = getTexture(scene, objectType, objectNumber);
    float x = TC.x * (texture.width - 1);
    float y = TC.y * (texture.height - 1);
    int i = int(x);
    int j = int(y);
    float alpha = x - i;
    float beta = y - j;
    Vec3 px0 = texture.textureArray[i][j];
    Vec3 px1 = texture.textureArray[i+1][j];
    Vec3 px2 = texture.textureArray[i][j+1];
    Vec3 px3 = texture.textureArray[i+1][j+1];
    return (px0 * (1 - alpha) * (1 - beta) + px1 * alpha * (1 - beta) + px2 * (1 - alpha) * beta + px3 * alpha * beta);
}