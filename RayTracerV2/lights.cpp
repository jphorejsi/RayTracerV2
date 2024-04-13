#include "lights.h"
#include "textures.h"
#include "bump.h"
#include "scene.h"

const MaterialType& getMaterial(const SceneType& scene, const std::string objectType, const int objectNumber) {
    if (objectType == "Sphere") {
        return scene.materials[scene.spheres[objectNumber].materialId];
    }
    else return scene.materials[scene.triangles[objectNumber].materialId];
}

float shadowFlag(const SceneType &scene, const RayType &ray, const LightType &light, const int excliusion) {
    float sum = 1;
    std::string objectType;
    int objectNumber;
    float rayT;
    for (auto s : scene.spheres) {
        if (raySphereIntersection(scene, s.Id, ray)) {
            sum = sum * (1 - scene.materials[s.materialId].alpha); //or alpha
        }
    }
    for (int i = 0; i < scene.trianglesSize; i++) {
        if (rayTriangleIntersection(scene, scene.triangles[i].Id, ray)) {
            return 0;
        }
    }
    return sum;
}

Vec3 lightShade(SceneType &scene, const RayType &ray, Vec3 &intersectionPoint, const LightType &light, const std::string objectType, const int objectNumber) {
    Vec3 N = getNormal(scene, objectType, objectNumber, intersectionPoint);
    Vec3 L;
    Vec3 diffuse, specular;
    float lightDistance;
    if (isTextured(scene, objectType, objectNumber) && isNormalMapped(scene, objectType, objectNumber)) {
        N = normalMapping(scene, objectType, objectNumber, intersectionPoint);
    }
    if (light.type == 0) { //if light type is directional
        L = (light.position * -1.0f).normal();
        lightDistance = FLT_MAX;
    }
    else {  //point
        L = (light.position - intersectionPoint).normal();
        lightDistance = sqrt(pow(light.position.x - intersectionPoint.x, 2) + pow(light.position.y - intersectionPoint.y, 2) + pow(light.position.z - intersectionPoint.z, 2));
    }
    Vec3 v = (ray.direction * -1).normal();
    Vec3 H = (L + v).normal();
    if (objectType == "Sphere") {
        diffuse = scene.materials[scene.spheres[objectNumber].materialId].od * scene.materials[scene.spheres[objectNumber].materialId].kd * (std::max(float(0), N.dot(L)));
        specular = scene.materials[scene.spheres[objectNumber].materialId].os * scene.materials[scene.spheres[objectNumber].materialId].ks * pow(std::max(float(0), N.dot(H)), scene.materials[scene.spheres[objectNumber].materialId].n);
    }
    else {
        diffuse = scene.materials[scene.triangles[objectNumber].materialId].od * scene.materials[scene.triangles[objectNumber].materialId].kd * (std::max(float(0), N.dot(L)));
        specular = scene.materials[scene.triangles[objectNumber].materialId].os * scene.materials[scene.triangles[objectNumber].materialId].ks * pow(std::max(float(0), N.dot(H)), scene.materials[scene.triangles[objectNumber].materialId].n);
    }
    Vec3 color = diffuse + specular;
    color.x *= light.color.x;
    color.y *= light.color.y;
    color.z *= light.color.z;
    float shadowF = float(shadowFlag(scene, RayType(intersectionPoint, L), light, objectNumber));
    return color * shadowF;
}


float lightAttenuation(const Vec3 &intersectionPoint, const AttLightType &attLight) {
    float f = 1.0;
    if (attLight.type == 1) {
        float dist = distance(intersectionPoint, attLight.position);
        f = 1.0 / (attLight.c1 + attLight.c2 * dist + attLight.c3 * dist * dist);
        f = std::min(float(1.0), f);
    }
    return f;
}

float depthCueing(const SceneType& scene, const DepthCueType& depthCue, const Vec3& intersectionPoint, const Vec3& eye) {
    float alpha = 1.0;
    float dist = distance(intersectionPoint, eye);
    if (dist < scene.depthCue.distanceMin) {
        alpha = scene.depthCue.alphaMax;
    }
    else if (dist < scene.depthCue.distanceMax) {
        alpha = scene.depthCue.alphaMin + (scene.depthCue.alphaMax - scene.depthCue.alphaMin) * (scene.depthCue.distanceMax - dist) / (scene.depthCue.distanceMax - scene.depthCue.distanceMin);
    }
    else alpha = scene.depthCue.alphaMin;
    return alpha;
}

Vec3 shadeRay(SceneType& scene, const std::string objectType, const int objectNumber, Vec3& intersectionPoint, const RayType& ray) {
    Vec3 L, H, v, N, newVec, diffuse, specular, color;
    float f = 1.0f;
    MaterialType material = getMaterial(scene, objectType, objectNumber);
    if (isTextured(scene, objectType, objectNumber)) {
        material.od = getColor(scene, objectType, objectNumber, intersectionPoint);
        if (objectType == "Sphere") {   //replace material od with texture od
            scene.materials[scene.spheres[objectNumber].materialId].od = material.od;
        }
        else if (objectType == "Triangle") {
            scene.materials[scene.triangles[objectNumber].materialId].od = material.od;
        }
    }
    color = material.od * material.ka;
    for (int i = 0; i < scene.lights.size(); i++) { //illumination for normal light
        color += lightShade(scene, ray, intersectionPoint, scene.lights[i], objectType, objectNumber);
    }
    for (int i = 0; i < scene.attLights.size(); i++) {
        if (scene.attLights[i].type == 1) {
            f = lightAttenuation(intersectionPoint, scene.attLights[i]);
        }
        color += lightShade(scene, ray, intersectionPoint, scene.attLights[i], objectType, objectNumber) * f;
    }
    color.x = std::min(1.0f, color.x); color.y = std::min(1.0f, color.y); color.z = std::min(1.0f, color.z); //Clamp to 1.0
    if (scene.depthCue.enabled == true) {
        float dcf = depthCueing(scene, scene.depthCue, intersectionPoint, scene.eyePosition);
        color = color * dcf + scene.depthCue.dc * (1 - dcf);
    }
    return color;
}
