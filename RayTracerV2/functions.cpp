#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <tuple>
#include "functions.h"
#include "classes.h"
#include "bump.h"
#include "scene.h"



const MaterialType &getMaterial(const SceneType &scene, const std::string objectType, const int objectNumber) {
    if (objectType == "Sphere") {
        return scene.materials[scene.spheres[objectNumber].materialId];
    }
    else return scene.materials[scene.triangles[objectNumber].materialId];
}

std::tuple<std::string, int, float> intersectionCheck(const SceneType &scene, const RayType &ray, const int exclude_id) {
    float min_t = 100000;
    float temp_t;
    int obj_idx = -1;
    std::string obj_type = "None";
    float A, B, C, D;
    float ray_t;
    float determinant;
    Vec3 p;
    Vec3 ray_center, obj_center;
    Vec3 dir;
    for (auto s : scene.spheres) {
        ray_center = ray.position;
        obj_center = s.position;
        dir = ray.direction;
        B = 2 * (dir.x * (ray_center.x - obj_center.x) +
            dir.y * (ray_center.y - obj_center.y) +
            dir.z * (ray_center.z - obj_center.z));
        C = pow(ray_center.x - obj_center.x, 2) +
            pow(ray_center.y - obj_center.y, 2) +
            pow(ray_center.z - obj_center.z, 2) -
            pow(s.radius, 2);
        determinant = pow(B, 2) - 4 * C;
        if (determinant > -1e-6) {
            temp_t = (-B - sqrt(determinant)) / 2;
            if (temp_t > 1e-6 && temp_t < min_t && s.Id != exclude_id) {
                min_t = temp_t;
                obj_idx = s.Id;
                obj_type = "Sphere";
            }
            temp_t = (-B + sqrt(determinant)) / 2;
            if (temp_t > 1e-6 && temp_t < min_t && s.Id != exclude_id) {
                min_t = temp_t;
                obj_idx = s.Id;
                obj_type = "Sphere";
            }
        }
    }
    for (auto t : scene.triangles) {
        ray_center = ray.position;
        dir = ray.direction;
        Vec3 p0 = scene.vertices[t.v0Index - 1];
        Vec3 p1 = scene.vertices[t.v1Index - 1];
        Vec3 p2 = scene.vertices[t.v2Index - 1];
        Vec3 e1 = p1 - p0;
        Vec3 e2 = p2 - p0;
        Vec3 n = e1.cross(e2).normal();
        A = n.x;
        B = n.y;
        C = n.z;
        D = -A * p0.x - B * p0.y - C * p0.z;
        determinant = A * dir.x + B * dir.y + C * dir.z;
        if (std::abs(determinant) < 1e-6) {
            continue;
        }
        ray_t = -(A * ray_center.x + B * ray_center.y + C * ray_center.z + D) / determinant;
        if (ray_t < 0) {
            continue;
        }
        p = ray.position + ray.direction * ray_t;
        Vec3 bayrcentric_coordinates = t.barycentric(scene, p);
        float alpha = bayrcentric_coordinates.x;
        float beta = bayrcentric_coordinates.y;
        float gamma = bayrcentric_coordinates.z;
        if (alpha > -1e-6 && alpha < 1 && beta > -1e-6 && beta < 1 && gamma > -1e-6 && gamma < 1) {
            if (ray_t < min_t && t.Id != exclude_id) {
                min_t = ray_t;
                obj_idx = t.Id;
                obj_type = "Triangle";
            }
        }
        else {
            continue;
        }
    }
    return std::make_tuple(obj_type, obj_idx, min_t);
}

float distance(const Vec3 &point1, const Vec3 &point2) {
    float sum = pow(point1.x - point2.x, 2)
        + pow(point1.y - point2.y, 2)
        + pow(point1.z - point2.z, 2);
    return sqrt(sum);
}

float depthCueing(const SceneType &scene, const DepthCueType &depthCue, const Vec3 &intersectionPoint, const Vec3 &eye) {
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