//#include "functions.h"

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


MaterialType getMaterial(SceneType scene, std::string objectType, int objectNumber) {
    if (objectType == "Sphere") {
        return scene.materials[scene.spheres[objectNumber].materialId];
    }
    else return scene.materials[scene.triangles[objectNumber].materialId];
}


std::tuple<std::string, int, float> intersectionCheck(SceneType scene, RayType ray, int exclusion) {
    float xc, yz, zc, yc, B, C, disc, t1, t2;
    SphereType sphere;
    int currentObj = -1;
    Vec3 intersectionPoint;
    float minT = FLT_MAX;
    std::string shape = "None";
    for (int i = 0; i < scene.spheres.size(); i++) {        //check for intersection with spheres
        sphere = scene.spheres[i];
        xc = ray.position.x - sphere.position.x;
        yc = ray.position.y - sphere.position.y;
        zc = ray.position.z - sphere.position.z;
        B = 2 * (ray.direction.x * (xc)+ray.direction.y * (yc)+ray.direction.z * (zc));
        C = (xc) * (xc)+(yc) * (yc)+(zc) * (zc)-sphere.radius * sphere.radius;
        disc = (B * B) - (4 * C);
        if (disc > 0) {
            t1 = (-B - sqrt(disc)) / 2;
            t2 = (-B + sqrt(disc)) / 2;
            if (t1 > 0 && t1 < minT && i != exclusion) {
                minT = t1;
                currentObj = i;
                shape = "Sphere";
            }
            else if (t2 > 0 && t2 < minT && i != exclusion) {
                minT = t2;
                currentObj = i;
                shape = "Sphere";
            }
            else continue;
        }
    }

    TriangleType triangle;
    Vec3 p0, p1, p2, e1, e2, n, p, bc;
    float A, D, alpha, beta, gamma, rayT;
    for (int i = 0; i < scene.triangles.size(); i++) {  //check for intersection with triangles
        triangle = scene.triangles[i];
        p0 = scene.vertices[triangle.v0Index - 1];
        p1 = scene.vertices[triangle.v1Index - 1];
        p2 = scene.vertices[triangle.v2Index - 1];
        e1 = p1 - p0;
        e2 = p2 - p0;
        n = normalize(cross(e1, e2));
        A = n.x;
        B = n.y;
        C = n.z;
        D = -A * p0.x - B * p0.y - C * p0.z;
        disc = A * ray.direction.x + B * ray.direction.y + C * ray.direction.z;
        if (std::abs(disc) == 0) {
            continue;
        }
        rayT = -(A * ray.position.x + B * ray.position.y + C * ray.position.z + D) / disc;
        if (rayT < 0) {
            continue;
        }
        p = ray.position + ray.direction * rayT;
        bc = triangle.barycentric(scene, p);
        alpha = bc.x;
        beta = bc.y;
        gamma = bc.z;
        if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1) {
            if (rayT < minT && i != exclusion) {
                minT = rayT;
                currentObj = i;
                shape = "Triangle";
            }
        }
        else continue;
    }
    return std::make_tuple(shape, currentObj, minT);
}

float depthCueing(SceneType scene, DepthCueType depthCue, Vec3 intersectionPoint, Vec3 eye) {
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