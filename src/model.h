#ifndef TDC_MODEL_H_
#define TDC_MODEL_H_

#include <vector>

#include "basetypes.h"
#include "vertex.h"
#include "triangle.h"

// A model is made of triangles
class Model {

 public:
  typedef boost::shared_ptr<Model> ShPtr;
  
  Model();
  ~Model();
  int triangle_count();
  Triangle::ShPtr get_triangle(int i);
  void add_triangle(Triangle::ShPtr t);

 private:
  std::vector<Triangle::ShPtr> tris_;
  
  DISALLOW_COPY_AND_ASSIGN(Model);
};

#endif
