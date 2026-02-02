#include <iostream>
#include <string>

#include "objdecoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "triangle.h"
#include "vec3f.h"

ObjDecoder::ObjDecoder() {
}

ObjDecoder::~ObjDecoder() {
}

Scene::ShPtr ObjDecoder::decode(const std::vector<char>& b) {

  int index = 0;
  int triangles = 0;

  std::vector<Vec3f::ShPtr> verts;
  std::vector<Vec3f::ShPtr> norms;
  std::vector<std::string> tokens;
  auto mesh = std::make_shared<Mesh>();

  while (index < b.size()) {
    tokens = Tokenize(b, index);

    if (tokens.size() > 0) {
      if (tokens[0] == "#") {
        // this line is a comment - skip it
      } else if (tokens[0] == "v") {
        // found a vertex
        auto x = std::stof(tokens[1]);
        auto y = std::stof(tokens[2]);
        auto z = std::stof(tokens[3]);
        auto v = std::make_shared<Vec3f>(x, y, z);
        verts.push_back(v);
      } else if (tokens[0] == "vn") {
        // found a normal
        auto x = std::stof(tokens[1]);
        auto y = std::stof(tokens[2]);
        auto z = std::stof(tokens[3]);
        auto n = std::make_shared<Vec3f>(x, y, z);
        norms.push_back(n);
      } else if (tokens[0] == "f") {
        // -1 to each of these because OBJ uses 1-based indexing
        auto x = std::stoi(tokens[1]) - 1;
        auto y = std::stoi(tokens[4]) - 1;
        auto z = std::stoi(tokens[7]) - 1;
        auto n = std::stoi(tokens[9]) - 1;
        auto t = std::make_shared<Triangle>(verts[x], verts[y], verts[z], norms[n]);
        mesh->add_triangle(t);
        triangles++;
      }
      index = newline_index(b, index+1);
    } else {
      break;
    }
  }

  std::cout << "Read " << triangles << " triangles" << std::endl;

  auto node = std::make_shared<Node>();
  node->set_mesh(mesh);

  auto scene = std::make_shared<Scene>();
  scene->add_node(node);

  return scene;
}

// Returns the index of the first character following a group of newline characters after the offset
unsigned int ObjDecoder::newline_index(const std::vector<char>& b, int offset) {

  int ni = offset;

  while (!(b[ni] == '\n' || b[ni] == '\r')) {
    ni++;
  }
  while (b[ni] == '\n' || b[ni] == '\r') {
    ni++;
  }

  return ni++;
}

// Returns true if c is a whitespace character
bool ObjDecoder::is_whitespace(char c) {
  return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

// Returns a collection of whitespace-separated character strings occuring between offset and the end of the
// line
std::vector<std::string> ObjDecoder::Tokenize(const std::vector<char>& b, int offset) {

  std::vector<std::string> tokens;

  std::string line;
  line.assign(&b[offset], &b[newline_index(b, offset)]);

  const std::string delimiters = "\t /\r\n";
  std::string::size_type pos = 0;
  while (pos != std::string::npos) {
    auto end = line.find_first_of(delimiters, pos);
    if (end == std::string::npos) {
      tokens.push_back(line.substr(pos));
      break;
    }
    tokens.push_back(line.substr(pos, end - pos));
    pos = end + 1;
  }

  return tokens;
}
