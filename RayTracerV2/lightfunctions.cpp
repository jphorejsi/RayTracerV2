#include "functions.h"
#include "classes.h"
#include "texturefunctions.h"
#include "lightfunctions.h"

int shadowStatus(SceneType scene, RayType ray, LightType light, int excliusion) {
    std::string objectType;
    int objectNumber;
    float rayT;
    std::tie(objectType, objectNumber, rayT) = intersectionCheck(scene, ray, excliusion);
    if (objectType != "None")
    {
        //std::cout << "something\n";
        if (light.type == 1)
        {
            float maxT = (light.position.x - ray.position.x) / ray.direction.x;
            if (rayT > 0 && rayT < maxT)
            {
                return 0;
            }
        }
        else
        {
            if (rayT > 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

Vec3 lightShade(SceneType scene, RayType ray, Vec3 intersectionPoint, LightType light, std::string objectType, int objectNumber) {
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
        L = normalize(light.position * -1.0f);
        lightDistance = FLT_MAX;
    }
    else {  //point
        L = normalize(light.position - intersectionPoint);
        lightDistance = sqrt(pow(light.position.x - intersectionPoint.x, 2) + pow(light.position.y - intersectionPoint.y, 2) + pow(light.position.z - intersectionPoint.z, 2));
    }
    Vec3 v = normalize(ray.direction * -1);
    Vec3 H = normalize(L + v);

    
    if (objectType == "Sphere") {
        diffuse = scene.materials[scene.spheres[objectNumber].materialId].od * scene.materials[scene.spheres[objectNumber].materialId].kd * (std::max(float(0), dot(N, L)));
        specular = scene.materials[scene.spheres[objectNumber].materialId].os * scene.materials[scene.spheres[objectNumber].materialId].ks * pow(std::max(float(0), dot(N, H)), scene.materials[scene.spheres[objectNumber].materialId].n);
    }
    else {
        diffuse = scene.materials[scene.triangles[objectNumber].materialId].od * scene.materials[scene.triangles[objectNumber].materialId].kd * (std::max(float(0), dot(N, L)));
        specular = scene.materials[scene.triangles[objectNumber].materialId].os * scene.materials[scene.triangles[objectNumber].materialId].ks * pow(std::max(float(0), dot(N, H)), scene.materials[scene.triangles[objectNumber].materialId].n);
    }



    Vec3 newVec = diffuse + specular;
    newVec.x = newVec.x * light.color.x; newVec.y = newVec.y * light.color.y; newVec.z = newVec.z * light.color.z;
    RayType ray2 = RayType(intersectionPoint, L);
    return newVec * float(shadowStatus(scene, ray2, light, objectNumber));
}

//bool shadow_check(SceneType scene, RayType ray, LightType light, int exclusion)
//{
//    std::string obj_type;
//    int obj_idx;
//    float ray_t; // material index
//    // loop for all objects
//    // check whether there is an intersection
//    std::tie(obj_type, obj_idx, ray_t) = intersectionCheck(scene, ray, exclusion);
//
//    if (obj_type != "None")
//    {
//        // if it is a point light source
//        // need further check whether the object is within the range between
//        // the starting point of the ray and the point light source
//        if (light.type == 1)
//        {
//            float max_t = (light.position.x - ray.position.x) / ray.direction.x;
//            if (ray_t > 1e-6 && ray_t < max_t)
//            {
//                return true;
//            }
//        }
//        else
//        {
//            // in the case of directional light source,
//            // as long as the returned t is positive, there is shadow
//            if (ray_t > 1e-6)
//            {
//                return true;
//            }
//        }
//    }
//
//    return false;
//}
//
//
//Vec3 lightShade(SceneType scene, RayType ray, Vec3 intersectionPoint, LightType light, std::string objectType, int objectNumber)
//{
//    // get the intersection point
//    Vec3 p = intersectionPoint;
//    Vec3 dir = ray.direction;
//    const MaterialType cur_material = getMaterial(scene, objectType, objectNumber);
//    Vec3 Od_lambda = cur_material.od;
//    Vec3 Os_lambda = cur_material.os;
//    Vec3 N = getNormal(scene, objectType, objectNumber, p);
//    // calculate vector L
//    Vec3 L;
//    // compute the coresponding color from the texture coordinate
//    if (isTextured(scene, objectType, objectNumber))
//    {
//        Od_lambda = getColor(scene, objectType, objectNumber, p);
//        if (isNormalMapped(scene, objectType, objectNumber))
//        {
//            // if the normal map option is enabled, modified the current surface normal
//            N = normalMapping(scene, objectType, objectNumber, p);
//        }
//    }
//    if (light.type == 1) // point light source
//    {
//        L = normalize(Vec3(light.position.x - p.x, light.position.y - p.y, light.position.z - p.z));
//    }
//    else // directional light source
//    {
//        L = normalize(Vec3(-light.position.x, -light.position.y, -light.position.z));
//    }
//    // calculate vector H
//    // get the vector v
//    Vec3 V = normalize(Vec3(-dir.x, -dir.y, -dir.z));
//    Vec3 H;
//    H = normalize(V + L);
//
//    // apply the phong illumination model
//    float Ir, Ig, Ib;    // illumination components for R, G and B
//    int shadow_flag = 1; // shadowing effect flag, 1 when not in shadow, 0 when in shadow
//
//    // check for shadowing effect
//    // cast a second ray forwarding from the intersection point
//    // to the point light source,
//    // and check for intersection with objects in the scene
//    RayType ray_second = RayType(p, L);
//    // check intersection, if intersected, set the flag to be 0
//    if (shadow_check(scene, ray_second, light, objectNumber))
//    {
//        shadow_flag = 0;
//    }
//
//    float term1 = std::max(float(0), dot(N, L));
//    float term2 = pow(std::max(float(0), dot(N, H)), cur_material.n);
//    Ir = shadow_flag * (cur_material.kd * Od_lambda.x * term1 + cur_material.ks * Os_lambda.x * term2);
//    Ig = shadow_flag * (cur_material.kd * Od_lambda.y * term1 + cur_material.ks * Os_lambda.y * term2);
//    Ib = shadow_flag * (cur_material.kd * Od_lambda.z * term1 + cur_material.ks * Os_lambda.z * term2);
//
//    return (Ir, Ig, Ib);
//}

float lightAttenuation(Vec3 intersectionPoint, AttLightType attLight) {
    float f = 1.0;
    if (attLight.type == 1) {
        float dist = distance(intersectionPoint, attLight.position);
        f = 1.0 / (attLight.c1 + attLight.c2 * dist + attLight.c3 * dist * dist);
        f = std::min(float(1.0), f);

    }
    return f;
}

Vec3 shadeRay(SceneType scene, std::string objectType, int objectNumber, Vec3 intersectionPoint, RayType ray) {
    Vec3 L, H, v, N, newVec, diffuse, specular, sum;
    float f = 1.0f;
    MaterialType material = getMaterial(scene, objectType, objectNumber);
    //sum = scene.spheres[objectNumber].m.od * scene.spheres[objectNumber].m.ka;  //calculate ambient
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

    if (scene.depthCue.enabled == true) {
        float dcf = depthCueing(scene, scene.depthCue, intersectionPoint, scene.eyePosition);
        sum = sum * dcf + scene.depthCue.dc * (1 - dcf);
    }
    return sum;
}
