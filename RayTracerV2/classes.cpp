#include "classes.h"


// Function definitions for operator overloads
Vec3 Vec3::operator+(const Vec3& other) const {
    return { x + other.x, y + other.y, z + other.z };
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return { x - other.x, y - other.y, z - other.z };
}

Vec3 Vec3::operator-() const {
    return { -x, -y, -z };
}

Vec3 Vec3::operator*(float scalar) const {
    return { x * scalar, y * scalar, z * scalar };
}

Vec3 Vec3::operator/(float scalar) const {
    // Ensure not dividing by zero
    if (scalar != 0.0f) {
        return { x / scalar, y / scalar, z / scalar };
    }
    else {
        // Handle division by zero (you might want to handle this case differently)
        // For now, just return a zero vector
        return { 0.0f, 0.0f, 0.0f };
    }
}
