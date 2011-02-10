#include "triangle.h"
#include "vertex.h"

Triangle::Triangle(Vertex::ShPtr v1, Vertex::ShPtr v2, Vertex::ShPtr v3, Vertex::ShPtr normal)
    : v1_(v1),
      v2_(v2),
      v3_(v3),
      norm_(normal) {
}

Triangle::~Triangle() {
}
