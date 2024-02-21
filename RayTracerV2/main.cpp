//Includes
#include "functions.h"
#include <fstream>
#include <sstream>
#include <iostream>

//global
SceneType scene;

#define M_PI 3.14159265358979323846

float depthCueing(DepthCueType depthCue, Vec3 intersectionPoint, Vec3 eye) {
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

float lightAttenuation(Vec3 intersectionPoint, AttLightType attLight) {
    float dist = distance(intersectionPoint, attLight.position);
    float f = 1.0 / (attLight.c1 + attLight.c2 * dist + attLight.c3 * dist * dist);
    f = std::min(float(1.0), f);
    return f;
}

int shadowStatus(Vec3 intersectionPoint, Vec3 L, int objectNumber, float lightDistance) {
    float xc, yc, zc, B, C, disc, t1, t2;
    for (int i = 0; i < scene.sphereCount; i++) {
        if (i != objectNumber) {
            xc = intersectionPoint.x - scene.spheres[i].position.x;
            yc = intersectionPoint.y - scene.spheres[i].position.y;
            zc = intersectionPoint.z - scene.spheres[i].position.z;
            B = 2 * (L.x * (xc)+L.y * (yc)+L.z * (zc));
            C = (xc) * (xc)+(yc) * (yc)+(zc) * (zc)-scene.spheres[i].radius * scene.spheres[i].radius; //here
            disc = (B * B) - (4 * C);
            if (disc > 0) { //adjust shadow corners
                t1 = (-B + sqrt(disc)) / (2);
                t2 = (-B - sqrt(disc)) / (2);
                if ((t1 > 0 && t1 < lightDistance) || (t2 > 0 && t2 < lightDistance)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

Vec3 phongLighting(SceneType scene, RayType ray, LightType light, int objectNumber, Vec3 intersectionPoint, Vec3 N, Vec3 v) {
    Vec3 L;
    float lightDistance;
    if (light.type == 0) { //if light type is directional
        L = normalize(light.position * -1.0f);
        lightDistance = FLT_MAX;
    }
    else {  //point
        L = normalize(light.position - intersectionPoint);
        lightDistance = sqrt(pow(light.position.x - intersectionPoint.x, 2) + pow(light.position.y - intersectionPoint.y, 2) + pow(light.position.z - intersectionPoint.z, 2));
    }
    Vec3 H = normalize(L + v);
    Vec3 diffuse = scene.spheres[objectNumber].m.od * scene.spheres[objectNumber].m.kd * (std::max(float(0), dot(N, L)));
    Vec3 specular = scene.spheres[objectNumber].m.os * scene.spheres[objectNumber].m.ks * pow(std::max(float(0), dot(N, H)), scene.spheres[objectNumber].m.n);
    Vec3 newVec = diffuse + specular;
    newVec.x = newVec.x * light.color.x; newVec.y = newVec.y * light.color.y; newVec.z = newVec.z * light.color.z;
    return newVec * float(shadowStatus(intersectionPoint, L, objectNumber, lightDistance));

}

Vec3 shadeRay(SceneType scene, int objectNumber, Vec3 intersectionPoint, RayType ray) {
    Vec3 L, H, v, N, newVec, diffuse, specular, sum;
    float f = 1.0f;
    sum = scene.spheres[objectNumber].m.od * scene.spheres[objectNumber].m.ka;  //calculate ambient
    v = normalize(ray.direction * -1);
    N = normalize((intersectionPoint - scene.spheres[objectNumber].position) / scene.spheres[objectNumber].radius);
    for (int i = 0; i < scene.lightCount; i++) {
        sum = sum + phongLighting(scene, ray, scene.lights[i], objectNumber, intersectionPoint, N, v);
    }
    for (int i = 0; i < scene.attLightCount; i++) {
        if (scene.attLights[i].type == 1) {
            f = lightAttenuation(intersectionPoint, scene.attLights[i]);
        }
        sum = sum + phongLighting(scene, ray, scene.attLights[i], objectNumber, intersectionPoint, N, v) * f;
    }


    sum.x = std::min(1.0f, sum.x); sum.y = std::min(1.0f, sum.y); sum.z = std::min(1.0f, sum.z); //Clamp to 1.0
    if (scene.depthCue.enabled == true) {
        float dcf = depthCueing(scene.depthCue, intersectionPoint, scene.eyePosition);
        sum = sum * dcf + scene.depthCue.dc * (1 - dcf);
    }
    return sum;
}



Vec3 traceRay(SceneType scene, RayType ray) {
    float min_d, xc, yz, zc, B, C, disc, t1, t2;
    int current_obj = -1;
    SphereType sphere;
    Vec3 intersectionPoint;
    min_d = FLT_MAX;
    for (int i = 0; i < scene.sphereCount; i++) {
        sphere = scene.spheres[i];
        float xc = ray.position.x - sphere.position.x;
        float yc = ray.position.y - sphere.position.y;
        float zc = ray.position.z - sphere.position.z;
        float B = 2 * (ray.direction.x * (xc)+ray.direction.y * (yc)+ray.direction.z * (zc));
        float C = (xc) * (xc)+(yc) * (yc)+(zc) * (zc)-sphere.radius * sphere.radius;
        float disc = (B * B) - (4 * C);
        if (disc > 0) {
            float t1 = (-B - sqrt(disc)) / 2;
            float t2 = (-B + sqrt(disc)) / 2;
            if (t1 > 0 && t1 < min_d) {
                min_d = t1;
                current_obj = i;
            }
            else if (t2 > 0 && t2 < min_d) {
                min_d = t2;
                current_obj = i;
            }
            else continue;
        }
    }
    if (current_obj != -1) {
        intersectionPoint = ray.direction * min_d + ray.position;
        return shadeRay(scene, current_obj, intersectionPoint, ray);
    }
    return scene.backgroundColor;
}



//parse to globals function
int parse(std::string filename) {
    std::ifstream file;
    file.open(filename);
    if (file.fail()) {
        std::cout << "file fail\n";
        return 0;
    }
    if (filename.substr(filename.size() - 4) != ".txt") {
        std::cout << "not a text file!\n";
        return 0;
    }
    std::string subs;
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string str_param[10];
    MaterialType newMaterial;
    while (buffer >> subs) {
        if (subs == "eye") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2];
            scene.eyePosition = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "viewdir") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2];
            scene.viewDirection = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "updir") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2];
            scene.upDirection = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "hfov") {
            buffer >> str_param[0];
            scene.fov = FovType(0, std::stof(str_param[0]));
        }
        else if (subs == "bkgcolor") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2];
            scene.backgroundColor = Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2]));
        }
        else if (subs == "imsize") {
            buffer >> str_param[0] >> str_param[1];
            scene.imageSize = ISizeType(std::stoi(str_param[0]), std::stoi(str_param[1]));
        }
        else if (subs == "mtlcolor") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6] >> str_param[7] >> str_param[8] >> str_param[9];
            newMaterial = MaterialType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), Vec3(std::stof(str_param[3]), std::stof(str_param[4]), std::stof(str_param[5])), std::stof(str_param[6]), std::stof(str_param[7]), std::stof(str_param[8]), std::stof(str_param[9]));
        }
        else if (subs == "sphere") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3];
            scene.spheres[scene.sphereCount] = SphereType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), newMaterial);
            scene.sphereCount++;
        }
        else if (subs == "light") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6];
            scene.lights[scene.lightCount] = LightType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), Vec3(std::stof(str_param[4]), std::stof(str_param[5]), std::stof(str_param[6])));
            scene.lightCount++;
        }
        else if (subs == "attlight") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6] >> str_param[7] >> str_param[8] >> str_param[9];
            scene.attLights[scene.attLightCount] = AttLightType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), Vec3(std::stof(str_param[4]), std::stof(str_param[5]), std::stof(str_param[6])), std::stof(str_param[7]), std::stof(str_param[8]), std::stof(str_param[9]));
            scene.attLightCount++;
        }
        else if (subs == "depthcueing") {
            buffer >> str_param[0] >> str_param[1] >> str_param[2] >> str_param[3] >> str_param[4] >> str_param[5] >> str_param[6];
            scene.depthCue = DepthCueType(Vec3(std::stof(str_param[0]), std::stof(str_param[1]), std::stof(str_param[2])), std::stof(str_param[3]), std::stof(str_param[4]), std::stof(str_param[5]), std::stof(str_param[6]));
            scene.depthCue.enabled = true;
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
            //if (i == 159 && j == 424) {
            point = scene.viewingWindow.ul + deltaH * (float)i + deltaV * (float)j;
            ray.direction = normalize(point - scene.eyePosition);
            vecColor = traceRay(scene, ray);
            arr[i].r = vecColor.x; arr[i].g = vecColor.y; arr[i].b = vecColor.z;
            outfile << int(arr[i].r * 255) << " " << int(arr[i].g * 255) << " " << int(arr[i].b * 255) << "\n";
            //}
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
