#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include "classes.h"

bool isNumber(const std::string& s);
Vec3 cross(Vec3 u, Vec3 v);
float magnitude(const Vec3& v);
Vec3 normalize(Vec3 v);
float dot(Vec3 u, Vec3 v);
float distance(Vec3 u, Vec3 v);

#endif PART_1_H