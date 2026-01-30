#ifndef TDC_VERTEX_H_
#define TDC_VERTEX_H_

#include <memory>

#include "basetypes.h"

// Basic object for representing a vertex in 3 dimensions
class Vertex {
 public: 
  typedef std::shared_ptr<Vertex> ShPtr;
  
  float x_, y_, z_;
  
  Vertex(float, float, float);
  ~Vertex();
  
 private:
  DISALLOW_COPY_AND_ASSIGN(Vertex);
};

#endif
