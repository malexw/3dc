#include <fstream>
#include <iostream>
#include <string>

#include "objencoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "vec3f.h"
#include "vertex_attribute.h"

ObjEncoder::ObjEncoder() {
}

ObjEncoder::~ObjEncoder() {
}

bool ObjEncoder::encode(const Scene& scene, const std::filesystem::path& output_path) {

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

bool ObjEncoder::encode_mesh(const Mesh& mesh, const std::filesystem::path& output_path) {

  std::ofstream out(output_path, std::ios::out | std::ios::binary);

  bool has_normals = mesh.has_attribute(VertexAttribute::Normal);
  bool has_texcoords = mesh.has_attribute(VertexAttribute::TexCoord0);

  // Write positions
  for (const auto& p : mesh.positions()) {
    out << "v " << p.x << " " << p.y << " " << p.z << "\n";
  }

  // Write texture coordinates
  if (has_texcoords) {
    for (const auto& t : mesh.texcoords0()) {
      out << "vt " << t.x << " " << t.y << "\n";
    }
  }

  // Write normals
  if (has_normals) {
    for (const auto& n : mesh.normals()) {
      out << "vn " << n.x << " " << n.y << " " << n.z << "\n";
    }
  }

  // Write faces with appropriate format based on which attributes are present
  for (const auto& tri : mesh.triangles()) {
    out << "f";
    for (int i = 0; i < 3; ++i) {
      // OBJ uses 1-based indexing
      uint32_t vi = tri[i] + 1;
      out << " ";
      if (has_normals && has_texcoords) {
        out << vi << "/" << vi << "/" << vi;
      } else if (has_normals) {
        out << vi << "//" << vi;
      } else if (has_texcoords) {
        out << vi << "/" << vi;
      } else {
        out << vi;
      }
    }
    out << "\n";
  }

  out.close();

  return true;
}
