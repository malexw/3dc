#ifndef TDC_MODEL_H_
#define TDC_MODEL_H_

#include <vector>

#include "basetypes.h"
#include "vertex.h"
#include "triangle.h"

/*
 * A model is an internal representation of a 3D object.
 */ 
class Model {

 public:
  typedef boost::shared_ptr<Model> ShPtr;
  
  Model();
  ~Model();
  
  // Returns the number of triangles that make up the object
  const int triangle_count() const;
  
  // Returns a boost::shared_pointer to the triangle at index i
  Triangle::ShPtr get_triangle(int i);
  
  // Intended to be called by decoders to add geometry to the model as the triangles are decoded.
  void add_triangle(Triangle::ShPtr t);

 private:
  std::vector<Triangle::ShPtr> tris_;
  
  DISALLOW_COPY_AND_ASSIGN(Model);
};

#endif
