#include <fstream>
#include <iostream>
#include <string>

#include "encoder.h"
#include "stlbencoder.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"

StlbEncoder::StlbEncoder() {
}

StlbEncoder::~StlbEncoder() {
}

bool StlbEncoder::encode(Model& model, std::string output_path) {
  
  std::ofstream out(output_path.c_str(), std::ios::out | std::ios::app | std::ios::binary);
  for (int i = 0; i < 0x50; ++i) {
    out << '\0';
  }
  
  int triangle_count = model.triangle_count();

  if (triangle_count > 0) {
    #pragma message ("Warning: use of float in a non-platform independent way")
    char * c = reinterpret_cast<char*> (&triangle_count);
    for (int j = 0; j < 4; ++j) {
      out << *c;
      c++;
    }
    
    for (int i = 0; i < triangle_count; ++i) {
      WriteStlbTriangle(out, model.get_triangle(i));
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

// Writes the contents of a Vertex to the output stream in IEEE 754 floating point number format
void StlbEncoder::WriteStlbVertex(std::ofstream& out, Vertex::ShPtr v) {
  #pragma message ("Warning: use of float in a non-platform independent way")
  char * c = reinterpret_cast<char*> (&(v->x_));
  for (int j = 0; j < 4; ++j) {
    out << *c;
    c++;
  }
  c = reinterpret_cast<char*> (&(v->y_));
  for (int j = 0; j < 4; ++j) {
    out << *c;
    c++;
  }
  c = reinterpret_cast<char*> (&(v->z_));
  for (int j = 0; j < 4; ++j) {
    out << *c;
    c++;
  }
}
