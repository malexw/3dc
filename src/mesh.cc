#include "mesh.h"
#include "triangle.h"
#include "vec3f.h"

Mesh::Mesh() {
}

Mesh::~Mesh() {
}

const std::string& Mesh::name() const {
  return name_;
}

void Mesh::set_name(const std::string& name) {
  name_ = name;
}

const int Mesh::triangle_count() const {
  return tris_.size();
}

Triangle::ShPtr Mesh::get_triangle(int i) const {
  return tris_.at(i);
}

const std::vector<Triangle::ShPtr>& Mesh::triangles() const {
  return tris_;
}

void Mesh::add_triangle(Triangle::ShPtr t) {
  tris_.push_back(t);
}
