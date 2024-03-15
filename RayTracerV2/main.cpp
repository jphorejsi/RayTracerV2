//Includes
//#include "sphere.h"
//#include "triangle.h"
//#
//#include <fstream>
//#include <sstream>

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <tuple>
#include "functions.h"
#include "classes.h"
#include "lightfunctions.h"
#include "texturefunctions.h"

//global
SceneType scene;

Vec3 traceRay(SceneType scene, RayType ray, int i, int j) {
    int currentObj = -1;
    std::string shape = "None";
    float rayT;
    std::tie(shape, currentObj, rayT) = intersectionCheck(scene, ray, -1);
    Vec3 intersectionPoint = ray.position + ray.direction * rayT;
    if (shape != "None") {
        //std::cout << i << " " << j << std::endl;
        return shadeRay(scene, shape, currentObj, intersectionPoint, ray);
    }
    return scene.backgroundColor;
}

 
int parse(std::string filename) {
    std::ifstream inputstream(filename, std::ios::in | std::ios::binary);
    if (!inputstream.is_open())
    {
        fprintf(stderr, "Could not open input stream with file %s\n", filename.c_str());
    }

    std::string subs;
    std::string line;
    std::string str_param[10];
    int int_var[10];
    //while (iss >> subs) {

    while (std::getline(inputstream, line)){
        subs = "";
        std::istringstream iss(line);
        iss >> subs;

        if (subs == "eye") {
            iss >> str_param[0] >> str_param[1] >> str_param[2];
            scene.eyePosition = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "viewdir") {
            iss >> str_param[0] >> str_param[1] >> str_param[2];
            scene.viewDirection = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "updir") {
            iss >> str_param[0] >> str_param[1] >> str_param[2];
            scene.upDirection = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "hfov") {
            iss >> str_param[0];
            scene.fov = FovType(0, std::stof(str_param[0]));
        }
        else if (subs == "bkgcolor") {
            iss >> str_param[0] >> str_param[1] >> str_param[2];
            scene.backgroundColor = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "imsize") {
            iss >> str_param[0] >> str_param[1];
            scene.imageSize = ISizeType(std::stoi(str_param[0]), std::stoi(str_param[1]));
        }
        else if (subs == "mtlcolor") {
            iss >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6] >> str_param[7] >> str_param[8] >> str_param[9];
            scene.materials.push_back(MaterialType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), Vec3(std::stof(str_param[3]), std::stof(str_param[4]), std::stof(str_param[5])), std::stof(str_param[6]), std::stof(str_param[7]), std::stof(str_param[8]), std::stof(str_param[9])));
        }
        else if (subs == "sphere") {
            iss >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3];
            scene.spheres.push_back(SphereType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), scene.materials.size() - 1, scene.textures.size() - 1));   //if -1 no material or texture material cannot be -1 texture can
        }
        else if (subs == "light") {
            iss >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6];
            scene.lights.push_back(LightType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), Vec3(std::stof(str_param[4]), std::stof(str_param[5]), std::stof(str_param[6]))));
        }
        else if (subs == "attlight") {
            iss >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6] >> str_param[7] >> str_param[8] >> str_param[9];
            scene.attLights.push_back(AttLightType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), Vec3(std::stof(str_param[4]), std::stof(str_param[5]), std::stof(str_param[6])), std::stof(str_param[7]), std::stof(str_param[8]), std::stof(str_param[9])));
        }
        else if (subs == "depthcueing") {
            iss >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6];
            scene.depthCue = DepthCueType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), std::stof(str_param[4]), std::stof(str_param[5]), std::stof(str_param[6]));
            scene.depthCue.enabled = true;
        }
        else if (subs == "texture") {
            iss >> str_param[0]; //filename
            std::ifstream textureiss(str_param[0], std::ios::in | std::ios::binary);
            textureiss >> str_param[0] >> str_param[0] >> str_param[1] >> str_param[2];
            TextureType texture = TextureType(stoi(str_param[0]), stoi(str_param[1]), stoi(str_param[2]));
            for (int j = 0; j < texture.height; j++) {
                for (int i = 0; i < texture.width; i++) {
                    textureiss >> str_param[0] >> str_param[1] >> str_param[2];
                    texture.textureArray[i][j].x = stoi(str_param[0]) * 1.0f / texture.maxValue; //r
                    texture.textureArray[i][j].y = stoi(str_param[1]) * 1.0f / texture.maxValue; //g
                    texture.textureArray[i][j].z = stoi(str_param[2]) * 1.0f / texture.maxValue; //b
                }
            }
            scene.textures.push_back(texture);
        }
        else if (subs == "v") {
            iss >> str_param[0] >> str_param[1] >> str_param[2];
            scene.vertices.push_back(Vec3(stof(str_param[0]), stof(str_param[1]), stof(str_param[2])));
        }
        else if (subs == "vn") {
            iss >> str_param[0] >> str_param[1] >> str_param[2];
            scene.normalVerticies.push_back(normalize(Vec3(stof(str_param[0]), stof(str_param[1]), stof(str_param[2]))));
        }
        else if (subs == "vt") {
            iss >> str_param[0] >> str_param[1];
            scene.textureCordinates.push_back(Vec2(stof(str_param[0]), stof(str_param[1])));
        }
        else if (subs == "f") {
            TriangleType newTriangle;
            if (sscanf_s(line.c_str(), "f %d %d %d", int_var, int_var + 1, int_var + 2) == 3) {
                newTriangle = TriangleType(scene.materials.size() -1, scene.textures.size() - 1, scene.bumps.size() - 1, int_var[0], int_var[1], int_var[2]);
            }
            else if (sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5) == 6) {
                newTriangle = TriangleType(scene.materials.size() - 1, scene.textures.size() - 1, scene.bumps.size() - 1, int_var[0], int_var[2], int_var[4], true, int_var[1], int_var[3], int_var[5]);
            }
            else if (sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5) == 6) { //wrong index
                newTriangle = TriangleType(scene.materials.size() - 1, scene.textures.size() - 1, scene.bumps.size() - 1, int_var[0], int_var[2], int_var[4]);
                newTriangle.textureMapped = true;
                newTriangle.vt0Index = int_var[1];
                newTriangle.vt1Index = int_var[3];
                newTriangle.vt2Index = int_var[5];
            }
            else if (sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5, int_var + 6, int_var + 7, int_var + 8) == 9) {
                newTriangle = TriangleType(scene.materials.size() - 1, scene.textures.size() - 1, scene.bumps.size() - 1, int_var[0], int_var[3], int_var[6], true, int_var[2], int_var[5], int_var[8], true, int_var[1], int_var[4], int_var[7]);
            }
            scene.triangles.push_back(newTriangle);
        }
    }
    return 1;
}


int raycast(std::string filename) {
    ColorType* arr = new ColorType[scene.imageSize.width * scene.imageSize.height];  // 2. Define an array of sufficent size to store the color values
    float aspect_ratio = (float)scene.imageSize.width / (float)scene.imageSize.height;  // define viewing window
    scene.viewingWindow.width = 2 * 5 * (tan(.5 * (scene.fov.horizontal * M_PI / 180.0)));
    scene.viewingWindow.height = scene.viewingWindow.width / aspect_ratio;
    Vec3 u = cross(scene.viewDirection, scene.upDirection); // calculate u and v
    Vec3 v = cross(u, scene.viewDirection);
    u = normalize(u);
    v = normalize(v);
    Vec3 u2 = u * (float)(scene.viewingWindow.width / 2);
    Vec3 v2 = v * (float)(scene.viewingWindow.height / 2);
    scene.viewDirection = normalize(scene.viewDirection);
    Vec3 firsthalf = scene.eyePosition + scene.viewDirection * (float)5;
    scene.viewingWindow.ul = firsthalf - u2 + v2;
    scene.viewingWindow.ur = firsthalf + u2 + v2;
    scene.viewingWindow.ll = firsthalf - u2 - v2;
    scene.viewingWindow.lr = firsthalf + u2 - v2;

    Vec3 deltaH = (scene.viewingWindow.ur - scene.viewingWindow.ul) / (float)(scene.imageSize.width - 1); //maybe remove -1
    Vec3 deltaV = (scene.viewingWindow.ll - scene.viewingWindow.ul) / (float)(scene.imageSize.height - 1);

    //get and set file information
    std::string newfile = filename;
    newfile.pop_back(); newfile.pop_back(); newfile.pop_back(); //remove "txt"
    newfile.append("ppm");

    std::ofstream outfile(newfile);
    outfile << "P3" << "\n" << scene.imageSize.width << " " << scene.imageSize.height << "\n" << "255\n";

    Vec3 point;
    RayType ray;
    Vec3 vecColor;
    ray.position = scene.eyePosition;
    for (int j = 0; j < scene.imageSize.height; j++) {  // 4. for each pixel in the output image
        for (int i = 0; i < scene.imageSize.width; i++) {
            //if (j == 256 && i == 102) {
            //std::cout << i << " " << j << std::endl;
            point = scene.viewingWindow.ul + deltaH * (float)i + deltaV * (float)j;
            ray.direction = normalize(point - scene.eyePosition);
            vecColor = traceRay(scene, ray, i, j);
            arr[i].r = vecColor.x; arr[i].g = vecColor.y; arr[i].b = vecColor.z;
            outfile << int(arr[i].r * 255) << " " << int(arr[i].g * 255) << " " << int(arr[i].b * 255) << "\n";
            //}
            //else outfile << int(255) << " " << int(255) << " " << int(255) << "\n";
        }
    }
    outfile.close();
    std::cout << "checck";
    return 1;
}


int main(int argc, char** argv) {
    parse("test.txt");
    std::cout << "test\n";
    raycast("test.txt");
    std::cout << "done\n";
    return 1;
}
