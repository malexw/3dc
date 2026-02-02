#include "scene.h"

Scene::Scene() {
}

Scene::~Scene() {
}

const std::string& Scene::name() const {
  return name_;
}

void Scene::set_name(const std::string& name) {
  name_ = name;
}

const int Scene::node_count() const {
  return nodes_.size();
}

Node::ShPtr Scene::get_node(int i) const {
  return nodes_.at(i);
}

const std::vector<Node::ShPtr>& Scene::nodes() const {
  return nodes_;
}

void Scene::add_node(Node::ShPtr n) {
  nodes_.push_back(n);
}
