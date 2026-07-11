#ifndef TDC_SCENE_H_
#define TDC_SCENE_H_

#include <string>
#include <vector>

#include "tdc/material.h"
#include "tdc/node.h"

namespace tdc {

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

  const int node_count() const;
  Node::ShPtr get_node(int i) const;
  const std::vector<Node::ShPtr>& nodes() const;
  void add_node(Node::ShPtr n);

  const int material_count() const;
  Material::ShPtr get_material(int i) const;
  const std::vector<Material::ShPtr>& materials() const;
  void add_material(Material::ShPtr m);

 private:
  std::string name_;
  std::vector<Node::ShPtr> nodes_;
  std::vector<Material::ShPtr> materials_;

  Scene(const Scene&) = delete;
  Scene& operator=(const Scene&) = delete;
};

}  // namespace tdc

#endif
