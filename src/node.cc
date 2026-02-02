#include "node.h"

Node::Node() {
  set_identity();
}

Node::~Node() {
}

const std::string& Node::name() const {
  return name_;
}

void Node::set_name(const std::string& name) {
  name_ = name;
}

Mesh::ShPtr Node::mesh() const {
  return mesh_;
}

void Node::set_mesh(Mesh::ShPtr mesh) {
  mesh_ = mesh;
}

const std::array<float, 16>& Node::matrix() const {
  return matrix_;
}

void Node::set_matrix(const std::array<float, 16>& m) {
  matrix_ = m;
}

void Node::set_identity() {
  matrix_.fill(0.0f);
  matrix_[0]  = 1.0f;
  matrix_[5]  = 1.0f;
  matrix_[10] = 1.0f;
  matrix_[15] = 1.0f;
}
