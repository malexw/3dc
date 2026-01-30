#include <fstream>
#include <iostream>
#include <string>

#include "objencoder.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"

ObjEncoder::ObjEncoder() {
}

ObjEncoder::~ObjEncoder() {
}

/*
 * This is absolutely the stupidest way of writing an OBJ that could possibly work. Will come back and change
 * to minimize file size later
 */
bool ObjEncoder::encode(Model& model, std::string output_path) {

  int triangle_count = model.triangle_count();
  std::ofstream out(output_path.c_str(), std::ios::out | std::ios::binary);

  out << "# Exported to OBJ by 3dc\n";
  out << "# github.com/malexw/3dc\n";

  for (int i = 0; i < triangle_count; ++i) {
    WriteVerticies(out, model.get_triangle(i));
  }

  for (int i = 0; i < triangle_count; ++i) {
    WriteNormal(out, (model.get_triangle(i))->norm_);
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
  out << t->v1_->x_ << " ";
  out << t->v1_->y_ << " ";
  out << t->v1_->z_ << "\n";
  out << "v ";
  out << t->v2_->x_ << " ";
  out << t->v2_->y_ << " ";
  out << t->v2_->z_ << "\n";
  out << "v ";
  out << t->v3_->x_ << " ";
  out << t->v3_->y_ << " ";
  out << t->v3_->z_ << "\n";
}

void ObjEncoder::WriteNormal(std::ofstream& out, Vertex::ShPtr n) {

  out << "vn ";
  out << n->x_ << " ";
  out << n->y_ << " ";
  out << n->z_ << "\n";
}

void ObjEncoder::WriteFace(std::ofstream& out, int i) {

  out << "f ";
  out << (i*3)+1 << "//" << i+1 << " ";
  out << (i*3)+2 << "//" << i+1 << " ";
  out << (i*3)+3 << "//" << i+1 << "\n";
}
