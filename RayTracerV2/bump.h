#pragma once
#ifndef BUMP_H
#define BUMP_H
#include "classes.h"

struct BumpType {
	int width, height, maxValue;
	Vec3** bumpArray;
	BumpType(int widthI = 0, int heightI = 0, int maxValueI = 255) : width(widthI), height(heightI), maxValue(maxValueI) {
		this->bumpArray = new Vec3 * [width];
		for (int i = 0; i < width; i++) {
			this->bumpArray[i] = new Vec3[height];
		}
	}
};

#endif //BUMP_H