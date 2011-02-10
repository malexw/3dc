#include <iostream>

#include "stlbdecoder.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"

StlbDecoder::StlbDecoder() {
}

StlbDecoder::~StlbDecoder() {
}

Model::ShPtr StlbDecoder::decode(FileBlob& b) {
  
  Model::ShPtr model (new Model());
  int fsize = b.Size();
  
  unsigned int i = 0, triangle_count = 0;
  for ( ; i < 4; ++i) {
    triangle_count <<= 8;
    triangle_count += b[0x53-i];
  }
    
  for (i = 0x54; i < fsize; i += 0x32) {
    Vertex::ShPtr normal (new Vertex(btof(&b[i]), btof(&b[i+0x4]), btof(&b[i+0x8])));
    Vertex::ShPtr v1 (new Vertex(btof(&b[i+0xC]), btof(&b[i+0x10]), btof(&b[i+0x14])));
    Vertex::ShPtr v2 (new Vertex(btof(&b[i+0x18]), btof(&b[i+0x1C]), btof(&b[i+0x20])));
    Vertex::ShPtr v3 (new Vertex(btof(&b[i+0x24]), btof(&b[i+0x28]), btof(&b[i+0x2C])));
    
    Triangle::ShPtr tri (new Triangle(v1, v2, v3, normal));
    model->add_triangle(tri);
  }
  
  if (triangle_count == model->triangle_count() ) {
    std::cout << "Read " << triangle_count << " triangles" << std::endl;
  } else {
    std::cout << "Error: triangle count mismatch. Expected " << triangle_count;
    std::cout << " got " << model->triangle_count() << std::endl;
  }
  
  return model;
}

float StlbDecoder::btof(const char* start) {
  
  float ret;
  char * c = reinterpret_cast<char*> (&ret);
  for (int i = 0; i < 4; ++i) {
    c[i] = start[i];
  }
  
  return ret;
}
