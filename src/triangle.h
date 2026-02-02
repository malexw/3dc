#ifndef TDC_TRIANGLE_H_
#define TDC_TRIANGLE_H_

#include "vec3f.h"

// Basic object for representing geometry for a triangle, with normal
class Triangle {

 public:
  typedef std::shared_ptr<Triangle> ShPtr;

  Triangle(Vec3f::ShPtr v1, Vec3f::ShPtr v2, Vec3f::ShPtr v3, Vec3f::ShPtr normal);
  ~Triangle();
  Vec3f::ShPtr v1_, v2_, v3_;
  Vec3f::ShPtr norm_;

  Triangle(const Triangle&) = delete;
  Triangle& operator=(const Triangle&) = delete;
};

#endif
