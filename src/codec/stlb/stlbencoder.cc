#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "stlbencoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "vec3f.h"
#include "vertex_attribute.h"

static void write_uint32_le(std::ofstream& out, uint32_t value) {
  char bytes[4];
  bytes[0] = value & 0xFF;
  bytes[1] = (value >> 8) & 0xFF;
  bytes[2] = (value >> 16) & 0xFF;
  bytes[3] = (value >> 24) & 0xFF;
  out.write(bytes, 4);
}

static void write_float_le(std::ofstream& out, float value) {
  uint32_t bits;
  std::memcpy(&bits, &value, sizeof(bits));
  write_uint32_le(out, bits);
}

static void write_vec3f_le(std::ofstream& out, const Vec3f& v) {
  write_float_le(out, v.x);
  write_float_le(out, v.y);
  write_float_le(out, v.z);
}

StlbEncoder::StlbEncoder() {
}

StlbEncoder::~StlbEncoder() {
}

bool StlbEncoder::encode(const Scene& scene, const std::filesystem::path& output_path) {

  auto count = scene.node_count();

  if (count == 1) {
    auto mesh = scene.get_node(0)->mesh();
    if (mesh) {
      return encode_mesh(*mesh, output_path);
    }
    return false;
  }

  for (int i = 0; i < count; ++i) {
    auto mesh = scene.get_node(i)->mesh();
    if (mesh) {
      auto path = output_path.parent_path()
        / (output_path.stem().string() + "_" + std::to_string(i)
           + output_path.extension().string());
      if (!encode_mesh(*mesh, path)) {
        return false;
      }
    }
  }

  return true;
}

bool StlbEncoder::encode_mesh(const Mesh& mesh, const std::filesystem::path& output_path) {

  std::ofstream out(output_path, std::ios::out | std::ios::binary);

  // Write 80-byte header (zeros)
  for (int i = 0; i < 0x50; ++i) {
    out << '\0';
  }

  auto triangle_count = mesh.triangle_count();

  if (triangle_count > 0) {
    write_uint32_le(out, triangle_count);

    bool has_normals = mesh.has_attribute(VertexAttribute::Normal);
    const auto& positions = mesh.positions();
    const auto& normals = mesh.normals();

    for (const auto& tri : mesh.triangles()) {
      // Write face normal (use first vertex's normal, or zero)
      if (has_normals) {
        write_vec3f_le(out, normals[tri[0]]);
      } else {
        write_float_le(out, 0.0f);
        write_float_le(out, 0.0f);
        write_float_le(out, 0.0f);
      }

      // Write 3 vertices
      write_vec3f_le(out, positions[tri[0]]);
      write_vec3f_le(out, positions[tri[1]]);
      write_vec3f_le(out, positions[tri[2]]);

      // Write 2-byte attribute byte count (unused, set to 0)
      out << '\0' << '\0';
    }
    out.close();
  }

  return true;
}
