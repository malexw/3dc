#include "model.h"
#include "triangle.h"
#include "vertex.h"

Model::Model() {
}

Model::~Model() {
}

const int Model::triangle_count() const {
  return tris_.size();
}

Triangle::ShPtr Model::get_triangle(int i) {
  return tris_.at(i);
}

void Model::add_triangle(Triangle::ShPtr t) {
  tris_.push_back(t);
}
