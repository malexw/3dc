#ifndef TDC_TRANSFORM_H_
#define TDC_TRANSFORM_H_

#include <array>

#include "tdc/vec3f.h"

namespace tdc::transform {

using Mat4 = std::array<float, 16>;

// Create identity matrix
Mat4 identity();

// Create translation matrix
Mat4 translation(float x, float y, float z);

// Create rotation matrices (angles in degrees)
Mat4 rotation_x(float degrees);
Mat4 rotation_y(float degrees);
Mat4 rotation_z(float degrees);

// Create uniform scale matrix
Mat4 scale(float factor);

// Matrix multiplication: a * b
Mat4 multiply(const Mat4& a, const Mat4& b);

// Apply matrix to point (position, affected by translation)
Vec3f apply_point(const Mat4& m, const Vec3f& p);

// Apply matrix to vector (direction/normal, not affected by translation)
Vec3f apply_vector(const Mat4& m, const Vec3f& v);

}  // namespace tdc::transform

#endif
