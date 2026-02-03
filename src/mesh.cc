#include "mesh.h"

bool Mesh::has_attribute(VertexAttribute attr) const {
  switch (attr) {
    case VertexAttribute::Position:
      return !positions_.empty();
    case VertexAttribute::Normal:
      return !normals_.empty();
    case VertexAttribute::TexCoord0:
      return !texcoords0_.empty();
  }
  return false;
}

uint32_t Mesh::vertex_count() const {
  return positions_.size();
}

const std::vector<Vec3f>& Mesh::positions() const {
  return positions_;
}

void Mesh::set_positions(std::vector<Vec3f> positions) {
  positions_ = std::move(positions);
}

const std::vector<Vec3f>& Mesh::normals() const {
  return normals_;
}

void Mesh::set_normals(std::vector<Vec3f> normals) {
  normals_ = std::move(normals);
}

const std::vector<Vec2f>& Mesh::texcoords0() const {
  return texcoords0_;
}

void Mesh::set_texcoords0(std::vector<Vec2f> texcoords) {
  texcoords0_ = std::move(texcoords);
}

uint32_t Mesh::triangle_count() const {
  return triangles_.size();
}

const std::vector<std::array<uint32_t, 3>>& Mesh::triangles() const {
  return triangles_;
}

void Mesh::add_triangle(uint32_t v0, uint32_t v1, uint32_t v2) {
  triangles_.push_back({v0, v1, v2});
}

const std::string& Mesh::name() const {
  return name_;
}

void Mesh::set_name(const std::string& name) {
  name_ = name;
}
