#include <iostream>
#include <string>

#include "objdecoder.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"

ObjDecoder::ObjDecoder() {
}

ObjDecoder::~ObjDecoder() {
}

Model::ShPtr ObjDecoder::decode(FileBlob& b) {

  int index = 0;
  int triangles = 0;

  std::vector<Vertex::ShPtr> verts;
  std::vector<Vertex::ShPtr> norms;
  std::vector<std::string> tokens;
  Model::ShPtr model (new Model());

  while (index < b.Size()) {
    tokens = Tokenize(b, index);

    if (tokens.size() > 0) {
      if (tokens[0] == "#") {
        // this line is a comment - skip it
      } else if (tokens[0] == "v") {
        // found a vertex
        float x = std::stof(tokens[1]);
        float y = std::stof(tokens[2]);
        float z = std::stof(tokens[3]);
        Vertex::ShPtr v (new Vertex(x, y, z));
        verts.push_back(v);
      } else if (tokens[0] == "vn") {
        // found a normal
        float x = std::stof(tokens[1]);
        float y = std::stof(tokens[2]);
        float z = std::stof(tokens[3]);
        Vertex::ShPtr n (new Vertex(x, y, z));
        norms.push_back(n);
      } else if (tokens[0] == "f") {
        // -1 to each of these because OBJ uses 1-based indexing
        int x = std::stoi(tokens[1]) - 1;
        int y = std::stoi(tokens[4]) - 1;
        int z = std::stoi(tokens[7]) - 1;
        int n = std::stoi(tokens[9]) - 1;
        //std::cout << x << " " << y << " " << z << " " << n << std::endl;
        Triangle::ShPtr t (new Triangle(verts[x], verts[y], verts[z], norms[n]));
        model->add_triangle(t);
        triangles++;
        /*for (int i = 0; i < tokens.size(); ++i) {
          std::cout << i << ":" << " " << tokens[i] << " ";
        }
        std::cout << std::endl;*/
      }
      index = newline_index(b, index+1);
    } else {
      break;
    }
  }

  std::cout << "Read " << triangles << " triangles" << std::endl;

  return model;
}

// Returns the index of the first character following a group of newline characters after the offset
unsigned int ObjDecoder::newline_index(FileBlob& b, int offset) {

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
std::vector<std::string> ObjDecoder::Tokenize(FileBlob& b, int offset) {

  std::vector<std::string> tokens;

  std::string line;
  line.assign(&b[offset], &b[newline_index(b, offset)]);

  const std::string delimiters = "\t /\r\n";
  std::string::size_type pos = 0;
  while (pos != std::string::npos) {
    std::string::size_type end = line.find_first_of(delimiters, pos);
    if (end == std::string::npos) {
      tokens.push_back(line.substr(pos));
      break;
    }
    tokens.push_back(line.substr(pos, end - pos));
    pos = end + 1;
  }

  return tokens;
}
