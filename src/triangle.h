#ifndef TDC_TRIANGLE_H_
#define TDC_TRIANGLE_H_

#include "basetypes.h"
#include "vertex.h"

// Basic object for representing geometry for a triangle, with normal
class Triangle {

 public:
  typedef boost::shared_ptr<Triangle> ShPtr;
 
  Triangle(Vertex::ShPtr v1, Vertex::ShPtr v2, Vertex::ShPtr v3, Vertex::ShPtr normal);
  ~Triangle();
  Vertex::ShPtr v1_, v2_, v3_;
  Vertex::ShPtr norm_;

 private:

  DISALLOW_COPY_AND_ASSIGN(Triangle);
};

#endif
