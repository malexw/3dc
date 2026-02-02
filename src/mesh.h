#ifndef TDC_MESH_H_
#define TDC_MESH_H_

#include <string>
#include <vector>

#include "vec3f.h"
#include "triangle.h"

/*
 * A mesh is an internal representation of a 3D object's geometry.
 */
class Mesh {

 public:
  typedef std::shared_ptr<Mesh> ShPtr;

  Mesh();
  ~Mesh();

  const std::string& name() const;
  void set_name(const std::string& name);

  // Returns the number of triangles that make up the object
  const int triangle_count() const;

  // Returns a shared pointer to the triangle at index i
  Triangle::ShPtr get_triangle(int i) const;

  // Returns a const reference to the triangle vector for iteration
  const std::vector<Triangle::ShPtr>& triangles() const;

  // Intended to be called by decoders to add geometry to the mesh as the triangles are decoded.
  void add_triangle(Triangle::ShPtr t);

 private:
  std::string name_;
  std::vector<Triangle::ShPtr> tris_;

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
};

#endif
