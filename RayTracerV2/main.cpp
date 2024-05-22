#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "scene.h"
#include "sstream"

SceneType scene;

Vec3 traceRay(SceneType &scene, const RayType &ray, const int i, const int j) {
    Vec3 colorToReturn;
    int currentObj = -1;
    std::string shape = "None";
    float rayT;
    std::tie(shape, currentObj, rayT) = intersectionCheck(scene, ray, -1);
    Vec3 intersectionPoint = ray.position + ray.direction * rayT;
    if (shape != "None") {
        colorToReturn = shadeRay(scene, shape, currentObj, intersectionPoint, ray);
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
            //if (i == 304 && j == 335) {
            //     std::cout << "here\n";
            //}
            //i = 247; j = 158;
            point = scene.viewingWindow.ul + deltaH * (float)i + deltaV * (float)j;
            ray.direction = (point - scene.eyePosition).normal();
            vecColor = traceRay(scene, ray, i, j);
            arr[i].r = vecColor.x; arr[i].g = vecColor.y; arr[i].b = vecColor.z;
            outfile << int(arr[i].r * 255) << " " << int(arr[i].g * 255) << " " << int(arr[i].b * 255) << "\n";
        }
        std::cout << j << std::endl;
    }
    outfile.close();
    return 1;
}

int main(int argc, char** argv) {
    scene.autoSS = true;
    parse("test.txt", scene);
    raycast("test.txt");
    std::cout << "done\n";
    return 1;
}
