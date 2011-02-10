#include <fstream>
#include <iostream>
#include <string>

#include "stlencoder.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"

StlEncoder::StlEncoder() {
}

StlEncoder::~StlEncoder() {
}

bool StlEncoder::encode(Model& model, std::string output_path) {
  
  int triangle_count = model.triangle_count();
  std::ofstream out(output_path.c_str(), std::ios::out | std::ios::app | std::ios::binary);
  
  out << "solid output\r\n"; 

  for (int i = 0; i < triangle_count; ++i) {
    WriteStlTriangle(out, model.get_triangle(i));
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

// Writes the contents of a Vertex to the output stream
void StlEncoder::WriteStlVertex(std::ofstream& out, Vertex::ShPtr v) {
  
  out << v->x_ << " ";
  out << v->y_ << " ";
  out << v->z_ << "\r\n";
}
