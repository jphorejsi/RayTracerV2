#pragma once
#ifndef VEC_H
#define VEC_H

#include <cmath>

struct Vec2 {
	float x, y;
	Vec2(float xI = 0.0, float yI = 0.0) : x(xI), y(yI) {
	}
};

struct Vec3 {
    float x, y, z;

    // default constructor
    Vec3(float x_ = 0, float y_ = 0, float z_ = 0)
        : x(x_), y(y_), z(z_)
    {
    }

    // normalization
    Vec3 normal() const
    {
        float norm = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        return Vec3(this->x / norm, this->y / norm, this->z / norm);
    }

    // cross product
    Vec3 cross(const Vec3& v) const
    {
        return Vec3(this->y * v.z - this->z * v.y,
            this->z * v.x - this->x * v.z,
            this->x * v.y - this->y * v.x);
    }

    // dot product
    float dot(const Vec3& v) const
    {
        return (this->x * v.x + this->y * v.y + this->z * v.z);
    }

    // overload + operator
    Vec3 operator+(const Vec3& v) const
    {
        return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    // overload - operator
    Vec3 operator-(const Vec3& v) const
    {
        return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    // overload * operator, const multiple
    Vec3 operator*(float c) const
    {
        return Vec3(this->x * c, this->y * c, this->z * c);
    }

    // overload / operator, const multiple
    Vec3 operator/(float c) const
    {
        return Vec3(this->x / c, this->y / c, this->z / c);
    }
};

#endif //VEC_H