#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "stlencoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "triangle.h"
#include "vec3f.h"

StlEncoder::StlEncoder() {
}

StlEncoder::~StlEncoder() {
}

bool StlEncoder::encode(const Scene& scene, const std::filesystem::path& output_path) {

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

bool StlEncoder::encode_mesh(const Mesh& mesh, const std::filesystem::path& output_path) {

  std::ofstream out(output_path, std::ios::out | std::ios::binary);

  out << "solid output\r\n";

  for (const auto& tri : mesh.triangles()) {
    WriteStlTriangle(out, tri);
  }

  out << "endsolid output\r\n";

  out.close();

  return true;
}

// Writes the contents of a Triangle to the output stream
void StlEncoder::WriteStlTriangle(std::ofstream& out, Triangle::ShPtr t) {

  out << "facet normal ";
  WriteStlVertex(out, t->norm_);
  out << "   outer loop\r\n";
  out << "      vertex ";
  WriteStlVertex(out, t->v1_);
  out << "      vertex ";
  WriteStlVertex(out, t->v2_);
  out << "      vertex ";
  WriteStlVertex(out, t->v3_);
  out << "   endloop\r\n";
  out << "endfacet\r\n";
}

// Writes the contents of a Vec3f to the output stream
void StlEncoder::WriteStlVertex(std::ofstream& out, Vec3f::ShPtr v) {

  out << std::setprecision(15) << v->x << " ";
  out << std::setprecision(15) << v->y << " ";
  out << std::setprecision(15) << v->z << "\r\n";
}
