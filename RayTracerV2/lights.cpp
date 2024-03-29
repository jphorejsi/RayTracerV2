#include "lights.h"
#include "textures.h"
#include "bump.h"
#include "depthcue.h"
#include "scene.h"

const MaterialType& getMaterial(const SceneType& scene, const std::string objectType, const int objectNumber) {
    if (objectType == "Sphere") {
        return scene.materials[scene.spheres[objectNumber].materialId];
    }
    else return scene.materials[scene.triangles[objectNumber].materialId];
}

int shadowStatus(const SceneType &scene, const RayType &ray, const LightType &light, const int excliusion) {
    std::string objectType;
    int objectNumber;
    float rayT;
    std::tie(objectType, objectNumber, rayT) = intersectionCheck(scene, ray, excliusion);
    if (objectType != "None") {
        if (light.type == 1) {
            float maxT = (light.position.x - ray.position.x) / ray.direction.x;
            if (rayT > 0 && rayT < maxT) {
                return 0;
            }
        }
        else {
            if (rayT > 0) {
                return 0;
            }
        }
    }
    return 1;
}

Vec3 lightShade(const SceneType &scene, const RayType &ray, Vec3 &intersectionPoint, const LightType &light, const std::string objectType, const int objectNumber) {
    Vec3 N = getNormal(scene, objectType, objectNumber, intersectionPoint);
    Vec3 L;
    Vec3 Od, Os;
    Vec3 diffuse, specular;
    float lightDistance;
    if (isTextured(scene, objectType, objectNumber)) {
        Od = getColor(scene, objectType, objectNumber, intersectionPoint);
        if (isNormalMapped(scene, objectType, objectNumber)) {
            N = normalMapping(scene, objectType, objectNumber, intersectionPoint);
        }
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
    RayType raySecond = RayType(intersectionPoint, L);
    int shadowFlag = 1;
    if (shadowStatus(scene, raySecond, light, objectNumber) == 0) {
        shadowFlag = 0;
    }
    const MaterialType& cur_material = getMaterial(scene, objectType, objectNumber);
    float term1 = std::max(float(0), N.dot(L));
    float term2 = pow(std::max(float(0), N.dot(H)), cur_material.n);
    float Ir = shadowFlag * (cur_material.kd * cur_material.od.x * term1 + cur_material.ks * cur_material.os.x * term2);
    float Ig = shadowFlag * (cur_material.kd * cur_material.od.y * term1 + cur_material.ks * cur_material.os.y * term2);
    float Ib = shadowFlag * (cur_material.kd * cur_material.od.z * term1 + cur_material.ks * cur_material.os.z * term2);
    return Vec3(Ir, Ig, Ib);
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

Vec3 shadeRay(SceneType& scene, const std::string objectType, const int objectNumber, Vec3& intersectionPoint, const RayType& ray) {
    Vec3 L, H, v, N, newVec, diffuse, specular, sum;
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
    sum = material.od * material.ka;
    for (int i = 0; i < scene.lights.size(); i++) { //illumination for normal light
        Vec3 result = lightShade(scene, ray, intersectionPoint, scene.lights[i], objectType, objectNumber);
        sum = sum + result;
    }
    for (int i = 0; i < scene.attLights.size(); i++) {
        if (scene.attLights[i].type == 1) {
            f = lightAttenuation(intersectionPoint, scene.attLights[i]);
        }
        sum = sum + lightShade(scene, ray, intersectionPoint, scene.attLights[i], objectType, objectNumber) * f;
    }
    sum.x = std::min(1.0f, sum.x); sum.y = std::min(1.0f, sum.y); sum.z = std::min(1.0f, sum.z); //Clamp to 1.0
    //if (scene.depthCue.enabled == true) {
    //    float dcf = depthCueing(scene, scene.depthCue, intersectionPoint, scene.eyePosition);
    //    sum = sum * dcf + scene.depthCue.dc * (1 - dcf);
    //}
    return sum;
}
