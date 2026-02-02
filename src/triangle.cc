#include "triangle.h"
#include "vec3f.h"

Triangle::Triangle(Vec3f::ShPtr v1, Vec3f::ShPtr v2, Vec3f::ShPtr v3, Vec3f::ShPtr normal)
    : v1_(v1),
      v2_(v2),
      v3_(v3),
      norm_(normal) {
}

Triangle::~Triangle() {
}
