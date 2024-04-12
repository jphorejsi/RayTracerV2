#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "scene.h"
#include "sstream"

SceneType scene;
#define MAX_DEPTH 5

Vec3 recursiveTraceRay(SceneType &scene, const RayType &ray, int depth, bool flagEnter, float dist, std::string shape, int currentObj) {
    if (depth > MAX_DEPTH || shape == "None") {
        return Vec3(0, 0, 0);
    }
    // compute the reflection ray equation and the Fresnel reflectance coefficient
    const Vec3& ray_dir = ray.direction.normal();
    const MaterialType& mtl = getMaterial(scene, shape, currentObj);
    Vec3 p = ray.position;
    Vec3 N = getNormal(scene, shape, currentObj, p);
    float eta_i = 1.0; // incident from air
    float eta_t = mtl.eta;
    if (!flagEnter) {
        // if exiting, revsere normal vector N
        // and exchange eta_i and eta_t
        N = N * -1;
        eta_i = eta_t;
        eta_t = 1.0;
    }
    float F_0 = pow((eta_t - eta_i) / (eta_t + eta_i), 2);
    float F_r = F_0 + (1 - F_0) * pow(1 - N.dot(ray_dir), 5);
    // compute the reflective ray
    Vec3 R = (N * 2 * N.dot(ray_dir) - ray_dir).normal();
    std::string next_obj_type;
    int next_obj_idx;
    float ray_t; // material index
    RayType ray_reflected(p, R);
    // initialize the color for the reflective ray
    Vec3 res_color_reflect(0, 0, 0);
    // loop for all objects
    // check whether there is an intersection
    std::tie(next_obj_type, next_obj_idx, ray_t) = intersectionCheck(scene, ray_reflected);
    // new intersection point
    // new normal vector N, at the new intersection point
    Vec3 new_p = ray_reflected.position + ray_reflected.direction * ray_t;
    if (next_obj_type != "None")
    {
        res_color_reflect = shadeRay(scene, next_obj_type, next_obj_idx, new_p, ray_reflected);
    }
    // new incident ray
    Vec3 new_dir = ray_reflected.direction.normal() * -1;
    RayType new_ray_incident(new_p, new_dir);
    // recursive trace the reflective ray
    res_color_reflect = res_color_reflect * pow(F_r, depth) + recursiveTraceRay(scene, new_ray_incident, depth + 1, flagEnter, dist + ray_t, next_obj_type, next_obj_idx);

    // initialize the response color for the transmitted ray
    Vec3 res_color_transmit(0, 0, 0);
    // check the existence of the transmitted ray
    if (std::abs(1 - mtl.alpha) < 1e-6 || pow(N.dot(ray_dir), 2) < 1 - pow(eta_t / eta_i, 2))
    {
        // if opaque or total internal reflection
        // there is no tranmitted ray
        return res_color_reflect;
    }
    // compute the tranmitted ray
    Vec3 T = N * -1 * sqrt(1 - pow(eta_i / eta_t, 2) * (1 - pow(N.dot(ray_dir), 2))) + (N * N.dot(ray_dir) - ray_dir) * (eta_i / eta_t);
    RayType ray_tranmitted(p, T);
    // loop for all objects
    // check whether there is an intersection
    std::tie(next_obj_type, next_obj_idx, ray_t) = intersectionCheck(scene, ray_tranmitted);
    // new intersection point
    // new normal vector N, at the new intersection point
    Vec3 new_p_transmit = ray_tranmitted.position + ray_tranmitted.direction * ray_t;
    if (next_obj_type != "None")
    {
        res_color_transmit = shadeRay(scene, next_obj_type, next_obj_idx, new_p_transmit ,ray_tranmitted);
    }
    // new incident ray
    Vec3 new_dir_transmit = ray_tranmitted.direction.normal() * -1;
    RayType new_ray_incident_transmit(new_p_transmit, new_dir_transmit);
    // recursive trace the transmitted ray
    res_color_transmit = res_color_transmit * pow(1 - F_r, depth) * std::exp(-mtl.alpha * dist) + recursiveTraceRay(scene, new_ray_incident_transmit, depth + 1, !flagEnter, dist + ray_t, next_obj_type, next_obj_idx);
    return res_color_reflect + res_color_transmit;
}

Vec3 traceRayRecursive(RayType ray, SceneType scene ,int maxRecursion, float etaI) {

    if (maxRecursion < 0) {
        return Vec3(0.0, 0.0, 0.0);
    }

    // Trace ray for object intersections
    float time;
    //Object* object;

    Vec3 colorToReturn;
    int currentObj = -1;
    std::string shape = "None";
    float rayT;
    std::tie(shape, currentObj, rayT) = intersectionCheck(scene, ray, -1);
    Vec3 intersectionPoint = ray.position + ray.direction * rayT;

    if (shape != "None") {
        Vec3 N = getNormal(scene, shape, currentObj, intersectionPoint).normal();
        const MaterialType& mtl = getMaterial(scene, shape, currentObj);

        // Calculate intersection point
        Vec3 point = intersectionPoint;

        // Calculate surface normal at intersection point
        Vec3 n = N;

        // Calculate Frenzel reflectance
        Vec3 I = ray.direction.normal() * -1;
        float alpha = n.dot(I);
        if (alpha < 0) {
            n = n * -1;
            alpha = n.dot(I);
        }
        float etaT = mtl.eta;
        float F0 = pow((etaT - 1) / (etaT + 1), 2);
        float Fr = F0 + ((1 - F0) * pow(1 - alpha, 5));

        // Calculate illumination from object intersection point
        Vec3 illumination = shadeRay(scene, shape, currentObj, intersectionPoint, ray);

        // Recursively calculate reflection
        RayType R = RayType(point,  n * 2 * alpha - I);
        Vec3 reflection = traceRayRecursive(R, scene, maxRecursion - 1, etaI) * Fr;

        // Reset etaT to 1 if exiting material
        if (etaI == etaT) {
            etaT = 1;
        }

        // Recursively calculate refraction
        Vec3 refraction = Vec3(0, 0, 0);
        float discriminant = 1 - (pow(etaI / etaT, 2) * (1 - pow(alpha, 2)));
        if (discriminant >= 0) {
            Vec3 TDir = (((n * -1) * sqrt(discriminant)) + (((n * alpha) - I)) * (etaI / etaT)).normal();
            RayType T = RayType(point, TDir);
            refraction = traceRayRecursive(T, scene ,maxRecursion - 1, etaT) * (1 - Fr) * (1 - mtl.alpha);

        }

        // Set pixel to shaded intersection point
        Vec3 color = (illumination + reflection + refraction);
        if (color.x > 1) {
            color.x = 1;
        }
        if (color.y > 1) {
            color.y = 1;
        }
        if (color.z > 1) {
            color.z = 1;
        }

        if (color.x < 0) {
            color.x = 0;
        }
        if (color.y < 0) {
            color.y = 0;
        }
        if (color.z < 0) {
            color.z = 0;
        }


        return color;

    }
    else {

        // Set pixel to background color if no intersecion
        return scene.backgroundColor;
    }
}







Vec3 traceRay(SceneType &scene, const RayType &ray, const int i, const int j) {
    Vec3 colorToReturn;
    int currentObj = -1;
    std::string shape = "None";
    float rayT;
    std::tie(shape, currentObj, rayT) = intersectionCheck(scene, ray, -1);
    Vec3 intersectionPoint = ray.position + ray.direction * rayT;
    if (shape == "Triangle") {
        colorToReturn = shadeRay(scene, shape, currentObj, intersectionPoint, ray);
    }
    else if (shape == "Sphere") {
        //colorToReturn = recursiveTraceRay(scene, RayType(intersectionPoint, ray.direction.normal() * -1), 1, true, 0, shape, currentObj);
        //RayType ray = RayType(intersectionPoint, ray.direction.normal() * -1);
        colorToReturn = traceRayRecursive(ray, scene, 10, 1);
    }
    else return scene.backgroundColor;
    return colorToReturn;
}

int raycast(std::string filename) {
    ColorType* arr = new ColorType[scene.imageSize.x * scene.imageSize.y];
    float aspect_ratio = (float)scene.imageSize.x / (float)scene.imageSize.y;
    scene.viewingWindow.width = 2 * 5 * (tan(.5 * (scene.fov.x * 3.14159265358979323846 / 180.0)));
    scene.viewingWindow.height = scene.viewingWindow.width / aspect_ratio;
    Vec3 u = scene.viewDirection.cross(scene.upDirection);
    Vec3 v = u.cross(scene.viewDirection);
    u = u.normal();
    v = v.normal();
    Vec3 u2 = u * (float)(scene.viewingWindow.width / 2);
    Vec3 v2 = v * (float)(scene.viewingWindow.height / 2);
    scene.viewDirection = scene.viewDirection.normal();
    Vec3 firsthalf = scene.eyePosition + scene.viewDirection * (float)5;
    scene.viewingWindow.ul = firsthalf - u2 + v2;
    scene.viewingWindow.ur = firsthalf + u2 + v2;
    scene.viewingWindow.ll = firsthalf - u2 - v2;
    scene.viewingWindow.lr = firsthalf + u2 - v2;
    Vec3 deltaH = (scene.viewingWindow.ur - scene.viewingWindow.ul) / (float)(scene.imageSize.x - 1);
    Vec3 deltaV = (scene.viewingWindow.ll - scene.viewingWindow.ul) / (float)(scene.imageSize.y - 1);
    std::string newfile = filename;
    newfile.pop_back(); newfile.pop_back(); newfile.pop_back();
    newfile.append("ppm");
    std::ofstream outfile(newfile);
    outfile << "P3" << "\n" << scene.imageSize.x << " " << scene.imageSize.y << "\n" << "255\n";
    Vec3 point;
    RayType ray;
    Vec3 vecColor;
    ray.position = scene.eyePosition;
    for (int j = 0; j < scene.imageSize.y; j++) {
        for (int i = 0; i < scene.imageSize.x; i++) {
            //if (i == 678 && j == 626) {
                point = scene.viewingWindow.ul + deltaH * (float)i + deltaV * (float)j;
                ray.direction = (point - scene.eyePosition).normal();
                vecColor = traceRay(scene, ray, i, j);
                arr[i].r = vecColor.x; arr[i].g = vecColor.y; arr[i].b = vecColor.z;
                outfile << int(arr[i].r * 255) << " " << int(arr[i].g * 255) << " " << int(arr[i].b * 255) << "\n";
            //}
        }
        std::cout << j << std::endl;
    }
    outfile.close();
    return 1;
}

int main(int argc, char** argv) {
    //scene.autoSS = true;
    parse("test.txt", scene);
    raycast("test.txt");
    std::cout << "done\n";
    return 1;
}
