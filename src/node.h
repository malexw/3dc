#ifndef TDC_NODE_H_
#define TDC_NODE_H_

#include <array>
#include <string>

#include "mesh.h"

/*
 * A node holds a mesh and a transform within a scene.
 */
class Node {

 public:
  typedef std::shared_ptr<Node> ShPtr;

  Node();
  ~Node();

  const std::string& name() const;
  void set_name(const std::string& name);

  Mesh::ShPtr mesh() const;
  void set_mesh(Mesh::ShPtr mesh);

  const std::array<float, 16>& matrix() const;
  void set_matrix(const std::array<float, 16>& m);
  void set_identity();

 private:
  std::string name_;
  Mesh::ShPtr mesh_;
  std::array<float, 16> matrix_;

  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;
};

#endif
