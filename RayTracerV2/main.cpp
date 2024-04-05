#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "scene.h"
#include "sstream"

bool autoSS = true;
SceneType scene;

Vec3 traceRay(SceneType &scene, const RayType &ray, const int i, const int j) {
    int currentObj = -1;
    std::string shape = "None";
    float rayT;
    std::tie(shape, currentObj, rayT) = intersectionCheck(scene, ray, -1);
    Vec3 intersectionPoint = ray.position + ray.direction * rayT;
    if (shape != "None") {
        return shadeRay(scene, shape, currentObj, intersectionPoint, ray);
    }
    return scene.backgroundColor;
}

int parse(const std::string filename) {
    std::ifstream inputstream(filename, std::ios::in | std::ios::binary);
    if (!inputstream.is_open()) {
        fprintf(stderr, "Could not open input stream with file %s\n", filename.c_str());
    }
    std::string subs;
    std::string line;
    std::string str_param[10];
    int int_var[10];
    int sphereId = 0;
    int triangleId = 0;
    int bumpId = -1;
    int vertexIndex = 0;
    int normalVertexIndex = 0;
    int triangleIndex = 0;

    while(std::getline(inputstream, line)) { //allocate
        subs = "";
        std::istringstream iss(line);
        iss >> subs;
        if (subs == "v") {
            scene.verticesSize++;
            if (autoSS) {
                scene.normalVerticesSize++;
            }
        }
        if (subs == "f") {
            scene.trianglesSize++;
        }
        if (subs == "vn") {
            scene.normalVerticesSize++;
        }
    }
    scene.vertices = new Vec3[scene.verticesSize];
    scene.triangles = new TriangleType[scene.trianglesSize];
    scene.normalVertices = new Vec3[scene.normalVerticesSize];
    inputstream.close();    //reset to origin
    inputstream.open(filename, std::ios::in | std::ios::binary);
    while(std::getline(inputstream, line)){ //build
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
            scene.fov = Vec2(std::stof(str_param[0]), 0);
        }
        else if (subs == "bkgcolor") {
            iss >> str_param[0] >> str_param[1] >> str_param[2];
            scene.backgroundColor = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "imsize") {
            iss >> str_param[0] >> str_param[1];
            scene.imageSize = Vec2(std::stoi(str_param[0]), std::stoi(str_param[1]));
        }
        else if (subs == "mtlcolor") {
            iss >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6] >> str_param[7] >> str_param[8] >> str_param[9];
            scene.materials.push_back(MaterialType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), Vec3(std::stof(str_param[3]), std::stof(str_param[4]), std::stof(str_param[5])), std::stof(str_param[6]), std::stof(str_param[7]), std::stof(str_param[8]), std::stof(str_param[9])));
        }
        else if (subs == "sphere") {
            iss >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3];
            SphereType newSphere = SphereType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), scene.materials.size() - 1, scene.textures.size() - 1);   //if -1 no material or texture material cannot be -1 texture can
            newSphere.Id = sphereId;
            sphereId++;
            scene.spheres.push_back(newSphere);
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
            scene.vertices[vertexIndex] = Vec3(stof(str_param[0]), stof(str_param[1]), stof(str_param[2]));
            vertexIndex++;
        }
        else if (subs == "vn") {
            iss >> str_param[0] >> str_param[1] >> str_param[2];
            scene.normalVertices[normalVertexIndex] = Vec3(stof(str_param[0]), stof(str_param[1]), stof(str_param[2])).normal();
            normalVertexIndex++;
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
            else if (sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5) == 6) {
                newTriangle = TriangleType(scene.materials.size() - 1, scene.textures.size() - 1, scene.bumps.size() - 1, int_var[0], int_var[2], int_var[4]);
                newTriangle.textureMapped = true;
                newTriangle.vt0Index = int_var[1];
                newTriangle.vt1Index = int_var[3];
                newTriangle.vt2Index = int_var[5];
            }
            else if (sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5, int_var + 6, int_var + 7, int_var + 8) == 9) {
                newTriangle = TriangleType(scene.materials.size() - 1, scene.textures.size() - 1, scene.bumps.size() - 1, int_var[0], int_var[3], int_var[6], true, int_var[2], int_var[5], int_var[8], true, int_var[1], int_var[4], int_var[7]);
            }
            newTriangle.Id = triangleId;
            triangleId++;
            scene.triangles[triangleIndex] = newTriangle;
            triangleIndex++;
        }
        else if (subs == "bump") {
            iss >> str_param[0];
            bumpId++;
            std::ifstream bump_inputstream(str_param[0], std::ios::in | std::ios::binary);
            bump_inputstream >> str_param[1] >> int_var[0] >> int_var[1] >> int_var[2];
            BumpType bump = BumpType(int_var[0], int_var[1], int_var[2]);
            Vec3** checkerboard = bump.bumpArray;
            for (int j = 0; j < bump.height; j++) {
                for (int i = 0; i < bump.width; i++) {
                    bump_inputstream >> int_var[3] >> int_var[4] >> int_var[5];
                    checkerboard[i][j].x = (int_var[3] * 1.0 / bump.maxValue * 2 - 1);
                    checkerboard[i][j].y = (int_var[4] * 1.0 / bump.maxValue * 2 - 1);
                    checkerboard[i][j].z = (int_var[5] * 1.0 / bump.maxValue * 2 - 1);
                }
            }
            scene.bumps.push_back(bump);
        }
    }



    if (autoSS) {
        for (int i = 0; i < scene.trianglesSize; i++) { //give every triangle a surface normal
            TriangleType t = scene.triangles[i];
            Vec3 A = scene.vertices[t.v1Index - 1] - scene.vertices[t.v0Index - 1];
            Vec3 B = scene.vertices[t.v2Index - 1] - scene.vertices[t.v0Index - 1];
            scene.triangles[i].surfaceNormal = (A.cross(B)).normal();
            //std::cout << scene.triangles[i].surfaceNormal.x << " " << scene.triangles[i].surfaceNormal.y << " " << scene.triangles[i].surfaceNormal.z << "\n";
        }

       //The surface normal n0 at vertex v0 can be approximated by averaging the plane normals of each of the triangles that contain v0
        std::vector<Vec3> adjacentNormals;
        for (int i = 0; i < scene.verticesSize; i++) {
            std::vector<Vec3> adjacentNormals;
            Vec3 vertex = scene.vertices[i];
            for (int j = 0; j < scene.trianglesSize; j++) {
                //std::cout << j << std::endl;
                //if (j == 6319) {
                //    std::cout << "j\n";
                //}
                //std::cout << j << std::endl;
                scene.triangles[j].smoothShaded = true;
                TriangleType t = scene.triangles[j];
                if (vertex == scene.vertices[t.v0Index - 1] || vertex == scene.vertices[t.v1Index - 1] || vertex == scene.vertices[t.v2Index - 1]) {
                    adjacentNormals.push_back(t.surfaceNormal);
                }
            }
            for (int j = 0; j < adjacentNormals.size(); j++) {
                //std::cout << j << "\n";
                scene.normalVertices[i] = scene.normalVertices[i] + adjacentNormals[j];
            }
            scene.normalVertices[i] = scene.normalVertices[i] / adjacentNormals.size();
        }

        for (int i = 0; i < scene.verticesSize; i++) {
            scene.normalVertices[i] = scene.normalVertices[i].normal();
        }

        for (int i = 0; i < scene.trianglesSize; i++) {
            scene.triangles[i].vn0Index = scene.triangles[i].v0Index;
            scene.triangles[i].vn1Index = scene.triangles[i].v1Index;
            scene.triangles[i].vn2Index = scene.triangles[i].v2Index;
        }

        std::cout << "normal vertices calculated\n";
    }
    return 1;
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
            point = scene.viewingWindow.ul + deltaH * (float)i + deltaV * (float)j;
            ray.direction = (point - scene.eyePosition).normal();
            vecColor = traceRay(scene, ray, i, j);
            arr[i].r = vecColor.x; arr[i].g = vecColor.y; arr[i].b = vecColor.z;
            outfile << int(arr[i].r * 255) << " " << int(arr[i].g * 255) << " " << int(arr[i].b * 255) << "\n";
            //std::cout << j << std::endl;
        }
    }
    outfile.close();
    return 1;
}

int main(int argc, char** argv) {
    parse("test.txt");
    raycast("test.txt");
    std::cout << "done\n";
    return 1;
}
