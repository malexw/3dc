#include <cstdint>
#include <iostream>
#include <string>

#include "tdc/codec/stl/stldecoder.h"
#include "tdc/mesh.h"
#include "tdc/node.h"
#include "tdc/scene.h"
#include "tdc/vec3f.h"

namespace tdc {

StlDecoder::StlDecoder() {
}

StlDecoder::~StlDecoder() {
}

Scene::ShPtr StlDecoder::decode(const std::vector<char>& b,
                                const std::filesystem::path& source_path) {

  std::string header = "solid";
  std::string facet = "facet";
  std::string normal = "normal";
  std::string outer = "outer";
  std::string loop = "loop";
  std::string vertex = "vertex";
  std::string endloop = "endloop";
  std::string endfacet = "endfacet";
  std::string endsolid = "endsolid";

  int index = 0;
  uint32_t vert_index = 0;

  std::vector<Vec3f> positions;
  std::vector<Vec3f> normals;

  auto mesh = std::make_shared<Mesh>();
  auto node = std::make_shared<Node>();
  node->set_mesh(mesh);
  auto scene = std::make_shared<Scene>();
  scene->add_node(node);

  auto tokens = Tokenize(b, index);

  if (tokens[0].compare(header)) {
    std::cout << "Error: missing header" << std::endl;
    return scene;
  }

  index = newline_index(b, index+1);
  tokens = Tokenize(b, index);

  while (tokens[0].compare(endsolid)) {

    Vec3f face_normal(0, 0, 0);

    if (!(tokens[0].compare(facet))) {
      if (!(tokens[1].compare(normal))) {
        auto x = std::stof(tokens[2]);
        auto y = std::stof(tokens[3]);
        auto z = std::stof(tokens[4]);
        face_normal = Vec3f(x, y, z);
      } else {
        std::cout << "Error: missing 'normal'" << std::endl;
        return scene;
      }
    } else {
      std::cout << "Error: missing 'facet'" << std::endl;
      return scene;
    }

    index = newline_index(b, index+1);
    tokens = Tokenize(b, index);

    if (tokens[0].compare(outer) || tokens[1].compare(loop)) {
      std::cout << "Error: missing 'outer loop'" << std::endl;
      return scene;
    }

    for (int i = 3; i > 0; --i) {
      index = newline_index(b, index+1);
      tokens = Tokenize(b, index);

      if (!(tokens[0].compare(vertex))) {
        auto x = std::stof(tokens[1]);
        auto y = std::stof(tokens[2]);
        auto z = std::stof(tokens[3]);
        positions.emplace_back(x, y, z);
        // Each vertex gets the face normal
        normals.push_back(face_normal);
      } else {
        std::cout << "Error: missing 'vertex'" << std::endl;
        return scene;
      }
    }

    index = newline_index(b, index+1);
    tokens = Tokenize(b, index);

    if (tokens[0].compare(endloop)) {
      std::cout << "Error: missing 'endloop'" << std::endl;
      return scene;
    }

    index = newline_index(b, index+1);
    tokens = Tokenize(b, index);

    if (tokens[0].compare(endfacet)) {
      std::cout << "Error: missing 'endfacet'" << std::endl;
      return scene;
    }

    // Sequential indices: (0,1,2), (3,4,5), ...
    mesh->add_triangle(vert_index, vert_index + 1, vert_index + 2);
    vert_index += 3;

    index = newline_index(b, index+1);
    tokens = Tokenize(b, index);
  }

  mesh->set_positions(std::move(positions));
  mesh->set_normals(std::move(normals));

  std::cout << "Read " << mesh->triangle_count() << " triangles" << std::endl;

  return scene;
}

// Returns the index of the first non-whitespace character after offset
unsigned int StlDecoder::next_index(const std::vector<char>& b, int offset) {

  while (is_whitespace(b[offset])) {
    offset++;
  }

  return offset++;
}

// Returns the index of the first character following a group of newline characters after the offset
unsigned int StlDecoder::newline_index(const std::vector<char>& b, int offset) {

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
bool StlDecoder::is_whitespace(char c) {
  return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

//
std::vector<std::string> StlDecoder::Tokenize(const std::vector<char>& b, int offset) {

  std::vector<std::string> tokens;
  auto startOfString = next_index(b, offset);
  auto endOfString = startOfString;
  auto endOfLine = newline_index(b, offset);

  while (endOfString < endOfLine) {
    while (!is_whitespace(b[endOfString])) {
      endOfString++;
    }

    std::string str;
    str.assign(&b[startOfString], endOfString-startOfString);
    tokens.push_back(str);

    startOfString = next_index(b, endOfString);
    endOfString = startOfString;
  }

  return tokens;
}

}  // namespace tdc
