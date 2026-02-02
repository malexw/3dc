#include <fstream>
#include <iostream>
#include <string>

#include "objencoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "triangle.h"
#include "vec3f.h"

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

  auto triangle_count = mesh.triangle_count();
  std::ofstream out(output_path, std::ios::out | std::ios::binary);

  for (const auto& tri : mesh.triangles()) {
    WriteVerticies(out, tri);
  }

  for (const auto& tri : mesh.triangles()) {
    WriteNormal(out, tri->norm_);
  }

  for (int i = 0; i < triangle_count; ++i) {
    WriteFace(out, i);
  }

  out.close();

  return true;
}

// Writes all the verticies for a triangle
void ObjEncoder::WriteVerticies(std::ofstream& out, Triangle::ShPtr t) {

  out << "v ";
  out << t->v1_->x << " ";
  out << t->v1_->y << " ";
  out << t->v1_->z << "\n";
  out << "v ";
  out << t->v2_->x << " ";
  out << t->v2_->y << " ";
  out << t->v2_->z << "\n";
  out << "v ";
  out << t->v3_->x << " ";
  out << t->v3_->y << " ";
  out << t->v3_->z << "\n";
}

void ObjEncoder::WriteNormal(std::ofstream& out, Vec3f::ShPtr n) {

  out << "vn ";
  out << n->x << " ";
  out << n->y << " ";
  out << n->z << "\n";
}

void ObjEncoder::WriteFace(std::ofstream& out, int i) {

  out << "f ";
  out << (i*3)+1 << "//" << i+1 << " ";
  out << (i*3)+2 << "//" << i+1 << " ";
  out << (i*3)+3 << "//" << i+1 << "\n";
}
