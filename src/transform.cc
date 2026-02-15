#include "tdc/transform.h"

#include <cmath>

namespace tdc::transform {

namespace {
constexpr float kPi = 3.14159265358979323846f;
constexpr float kDegToRad = kPi / 180.0f;
}  // namespace

Mat4 identity() {
  return {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  };
}

Mat4 translation(float x, float y, float z) {
  return {
    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1
  };
}

Mat4 rotation_x(float degrees) {
  float rad = degrees * kDegToRad;
  float c = std::cos(rad);
  float s = std::sin(rad);
  return {
    1, 0,  0, 0,
    0, c, -s, 0,
    0, s,  c, 0,
    0, 0,  0, 1
  };
}

Mat4 rotation_y(float degrees) {
  float rad = degrees * kDegToRad;
  float c = std::cos(rad);
  float s = std::sin(rad);
  return {
     c, 0, s, 0,
     0, 1, 0, 0,
    -s, 0, c, 0,
     0, 0, 0, 1
  };
}

Mat4 rotation_z(float degrees) {
  float rad = degrees * kDegToRad;
  float c = std::cos(rad);
  float s = std::sin(rad);
  return {
    c, -s, 0, 0,
    s,  c, 0, 0,
    0,  0, 1, 0,
    0,  0, 0, 1
  };
}

Mat4 scale(float factor) {
  return {
    factor, 0,      0,      0,
    0,      factor, 0,      0,
    0,      0,      factor, 0,
    0,      0,      0,      1
  };
}

Mat4 multiply(const Mat4& a, const Mat4& b) {
  Mat4 result = {};
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      float sum = 0.0f;
      for (int k = 0; k < 4; ++k) {
        sum += a[row * 4 + k] * b[k * 4 + col];
      }
      result[row * 4 + col] = sum;
    }
  }
  return result;
}

Vec3f apply_point(const Mat4& m, const Vec3f& p) {
  // Row-major: result = M * [x, y, z, 1]^T
  float x = m[0] * p.x + m[1] * p.y + m[2] * p.z + m[3];
  float y = m[4] * p.x + m[5] * p.y + m[6] * p.z + m[7];
  float z = m[8] * p.x + m[9] * p.y + m[10] * p.z + m[11];
  return Vec3f(x, y, z);
}

Vec3f apply_vector(const Mat4& m, const Vec3f& v) {
  // Row-major: result = M * [x, y, z, 0]^T (no translation)
  float x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
  float y = m[4] * v.x + m[5] * v.y + m[6] * v.z;
  float z = m[8] * v.x + m[9] * v.y + m[10] * v.z;
  return Vec3f(x, y, z);
}

}  // namespace tdc::transform
