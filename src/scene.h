#ifndef TDC_SCENE_H_
#define TDC_SCENE_H_

#include <string>
#include <vector>

#include "node.h"

/*
 * A scene contains a collection of nodes, each of which holds a mesh and transform.
 */
class Scene {

 public:
  typedef std::shared_ptr<Scene> ShPtr;

  Scene();
  ~Scene();

  const std::string& name() const;
  void set_name(const std::string& name);

  // Returns the number of nodes in the scene
  const int node_count() const;

  // Returns a shared pointer to the node at index i
  Node::ShPtr get_node(int i) const;

  // Returns a const reference to the node vector for iteration
  const std::vector<Node::ShPtr>& nodes() const;

  // Adds a node to the scene
  void add_node(Node::ShPtr n);

 private:
  std::string name_;
  std::vector<Node::ShPtr> nodes_;

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;
};

#endif
