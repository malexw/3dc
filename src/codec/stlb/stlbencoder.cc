#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "encoder.h"
#include "stlbencoder.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"

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

bool StlbEncoder::encode(Model& model, std::string output_path) {
  
  std::ofstream out(output_path.c_str(), std::ios::out | std::ios::binary);
  for (int i = 0; i < 0x50; ++i) {
    out << '\0';
  }
  
  int triangle_count = model.triangle_count();

  if (triangle_count > 0) {
    write_uint32_le(out, triangle_count);
    
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
  write_float_le(out, v->x_);
  write_float_le(out, v->y_);
  write_float_le(out, v->z_);
}
