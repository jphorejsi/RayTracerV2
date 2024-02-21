#include "functions.h"
#include <cmath>
#include <iostream>
#include <algorithm>


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