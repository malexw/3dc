#ifndef TDC_MESH_H_
#define TDC_MESH_H_

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "material.h"
#include "vec2f.h"
#include "vec3f.h"
#include "vertex_attribute.h"

/*
 * A mesh is an internal representation of a 3D object's geometry.
 * Uses an indexed vertex model with typed attribute arrays.
 */
class Mesh {

 public:
  typedef std::shared_ptr<Mesh> ShPtr;

  // Query which attributes are present (checks if vector is non-empty)
  bool has_attribute(VertexAttribute attr) const;

  // Vertex count (length of positions array)
  uint32_t vertex_count() const;

  // Positions — required
  const std::vector<Vec3f>& positions() const;
  void set_positions(std::vector<Vec3f> positions);

  // Normals — optional
  const std::vector<Vec3f>& normals() const;
  void set_normals(std::vector<Vec3f> normals);

  // Texture coordinates — optional
  const std::vector<Vec2f>& texcoords0() const;
  void set_texcoords0(std::vector<Vec2f> texcoords);

  // Triangle indices
  uint32_t triangle_count() const;
  const std::vector<std::array<uint32_t, 3>>& triangles() const;
  void add_triangle(uint32_t v0, uint32_t v1, uint32_t v2);

  // Material
  Material::ShPtr material() const;
  void set_material(Material::ShPtr material);

  // Name
  const std::string& name() const;
  void set_name(const std::string& name);

  // Apply a 4x4 transform matrix to positions and normals
  void apply_transform(const std::array<float, 16>& m);

 private:
  std::string name_;
  Material::ShPtr material_;
  std::vector<Vec3f> positions_;
  std::vector<Vec3f> normals_;
  std::vector<Vec2f> texcoords0_;
  std::vector<std::array<uint32_t, 3>> triangles_;
};

#endif
