#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "encoder.h"
#include "stlbencoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "triangle.h"
#include "vec3f.h"

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
  for (int i = 0; i < 0x50; ++i) {
    out << '\0';
  }

  auto triangle_count = mesh.triangle_count();

  if (triangle_count > 0) {
    write_uint32_le(out, triangle_count);

    for (const auto& tri : mesh.triangles()) {
      WriteStlbTriangle(out, tri);
    }
    out.close();
  }

  return true;
}

// Writes the contents of a Triangle to the output stream
void StlbEncoder::WriteStlbTriangle(std::ofstream& out, Triangle::ShPtr t) {

    WriteStlbVertex(out, t->norm_);
    WriteStlbVertex(out, t->v1_);
    WriteStlbVertex(out, t->v2_);
    WriteStlbVertex(out, t->v3_);
    out << "  ";
}

// Writes the contents of a Vec3f to the output stream in IEEE 754 floating point number format
void StlbEncoder::WriteStlbVertex(std::ofstream& out, Vec3f::ShPtr v) {
  write_float_le(out, v->x);
  write_float_le(out, v->y);
  write_float_le(out, v->z);
}
