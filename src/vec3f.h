#ifndef TDC_VEC3F_H_
#define TDC_VEC3F_H_

// A 3-component float vector
class Vec3f {
 public:
  float x, y, z;

  Vec3f();
  Vec3f(float x, float y, float z);

  Vec3f operator-(const Vec3f& rhs) const;
  float dot(const Vec3f& rhs) const;
  Vec3f cross(const Vec3f& rhs) const;
};

#endif
