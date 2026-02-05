#include "vec3f.h"

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

float Vec3f::dot(const Vec3f& rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vec3f Vec3f::cross(const Vec3f& rhs) const {
  return Vec3f(
    y * rhs.z - z * rhs.y,
    z * rhs.x - x * rhs.z,
    x * rhs.y - y * rhs.x);
}
