#ifndef TDC_VEC3F_H_
#define TDC_VEC3F_H_

#include <memory>

// A 3-component float vector
class Vec3f {
 public:
  typedef std::shared_ptr<Vec3f> ShPtr;

  float x, y, z;

  Vec3f(float, float, float);
  ~Vec3f();

  Vec3f(const Vec3f&) = delete;
  Vec3f& operator=(const Vec3f&) = delete;
};

#endif
