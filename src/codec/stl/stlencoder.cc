#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "stlencoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "vec3f.h"
#include "vertex_attribute.h"

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

  bool has_normals = mesh.has_attribute(VertexAttribute::Normal);
  const auto& positions = mesh.positions();
  const auto& normals = mesh.normals();

  out << "solid output\r\n";

  for (const auto& tri : mesh.triangles()) {
    // Write face normal. Use the normal of the first vertex, or zero if no normals.
    out << "facet normal ";
    if (has_normals) {
      const auto& n = normals[tri[0]];
      out << std::setprecision(15) << n.x << " " << n.y << " " << n.z << "\r\n";
    } else {
      out << "0 0 0\r\n";
    }

    out << "   outer loop\r\n";
    for (int i = 0; i < 3; ++i) {
      const auto& v = positions[tri[i]];
      out << "      vertex ";
      out << std::setprecision(15) << v.x << " " << v.y << " " << v.z << "\r\n";
    }
    out << "   endloop\r\n";
    out << "endfacet\r\n";
  }

  out << "endsolid output\r\n";

  out.close();

  return true;
}
