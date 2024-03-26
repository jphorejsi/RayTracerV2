#include "textures.h"
#include "scene.h"

Vec2 getTextureCoordinate(const SceneType &scene, const std::string objectType, const int &objectNumber, Vec3 &intersectionPoint) {
    if (objectType == "Sphere") {
        return scene.spheres[objectNumber].textureCoordinate(intersectionPoint);
    }
    else return scene.triangles[objectNumber].textureCoordinate(scene, intersectionPoint);
}

Vec3 getNormal(const SceneType &scene, const std::string objectType, const int objectNumber, Vec3 &intersectionPoint) {
    if (objectType == "Sphere") {
        return scene.spheres[objectNumber].normal(intersectionPoint);
    }
    else return scene.triangles[objectNumber].normal(scene, intersectionPoint);
}

const TextureType &getTexture(const SceneType &scene, const std::string objectType, const int objectNumber) {
    if (objectType == "Sphere") {
        return scene.textures[scene.spheres[objectNumber].textureId];
    }
    else return scene.textures[scene.triangles[objectNumber].textureId];
}


Vec3 getColor(const SceneType &scene, const std::string objectType, const int objectNumber, Vec3 &intersectionPoint) {
    Vec2 TC = getTextureCoordinate(scene, objectType, objectNumber, intersectionPoint);
    TextureType texture = getTexture(scene, objectType, objectNumber);
    float x = TC.x * (texture.width - 1);
    float y = TC.y * (texture.height - 1);
    int i = int(x);
    int j = int(y);
    float alpha = x - i;
    float beta = y - j;
    Vec3 px0 = texture.textureArray[i][j];
    Vec3 px1 = texture.textureArray[i + 1][j];
    Vec3 px2 = texture.textureArray[i][j + 1];
    Vec3 px3 = texture.textureArray[i + 1][j + 1];
    return (px0 * (1 - alpha) * (1 - beta) + px1 * alpha * (1 - beta) + px2 * (1 - alpha) * beta + px3 * alpha * beta);
}

bool isTextured(const SceneType &scene, const std::string objectType, const int objectNumber) {
    if (objectType == "Sphere") {
        return (scene.spheres[objectNumber].textureId != -1);
    }
    else return (scene.triangles[objectNumber].textureId != -1);
}