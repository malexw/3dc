#include "vec3f.h"

#include <cmath>

Vec3f::Vec3f()
    : x(0.0f),
      y(0.0f),
      z(0.0f) {
}

Vec3f::Vec3f(float x, float y, float z)
    : x(x),
      y(y),
      z(z) {
}

Vec3f Vec3f::operator-(const Vec3f& rhs) const {
  return Vec3f(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vec3f Vec3f::operator+(const Vec3f& rhs) const {
  return Vec3f(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vec3f Vec3f::operator*(float scalar) const {
  return Vec3f(x * scalar, y * scalar, z * scalar);
}

float Vec3f::dot(const Vec3f& rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vec3f Vec3f::cross(const Vec3f& rhs) const {
  return Vec3f(
    y * rhs.z - z * rhs.y,
    z * rhs.x - x * rhs.z,
    x * rhs.y - y * rhs.x);
}

float Vec3f::length() const {
  return std::sqrt(x * x + y * y + z * z);
}

Vec3f Vec3f::normalized() const {
  float len = length();
  if (len > 0.0f) {
    return Vec3f(x / len, y / len, z / len);
  }
  return *this;
}
