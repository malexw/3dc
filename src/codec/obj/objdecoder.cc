#include <iostream>
#include <map>
#include <string>
#include <tuple>

#include "objdecoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "vec2f.h"
#include "vec3f.h"

ObjDecoder::ObjDecoder() {
}

ObjDecoder::~ObjDecoder() {
}

Scene::ShPtr ObjDecoder::decode(const std::vector<char>& b) {

  int index = 0;

  // Temporary arrays indexed by OBJ's 1-based indices (stored 0-based here)
  std::vector<Vec3f> tmp_positions;
  std::vector<Vec3f> tmp_normals;
  std::vector<Vec2f> tmp_texcoords;

  // Output arrays
  std::vector<Vec3f> out_positions;
  std::vector<Vec3f> out_normals;
  std::vector<Vec2f> out_texcoords;

  // Map from (pos_idx, norm_idx, tex_idx) to output vertex index
  // Using -1 to indicate "not present"
  std::map<std::tuple<int, int, int>, uint32_t> vertex_map;

  bool has_normals = false;
  bool has_texcoords = false;

  auto mesh = std::make_shared<Mesh>();

  // Helper to get or create a unified vertex index
  auto get_vertex_index = [&](int pi, int ni, int ti) -> uint32_t {
    auto key = std::make_tuple(pi, ni, ti);
    auto it = vertex_map.find(key);
    if (it != vertex_map.end()) {
      return it->second;
    }
    uint32_t idx = out_positions.size();
    out_positions.push_back(tmp_positions[pi]);
    if (ni >= 0) {
      out_normals.push_back(tmp_normals[ni]);
    }
    if (ti >= 0) {
      out_texcoords.push_back(tmp_texcoords[ti]);
    }
    vertex_map[key] = idx;
    return idx;
  };

  while (index < static_cast<int>(b.size())) {
    auto tokens = Tokenize(b, index);

    if (tokens.empty()) {
      break;
    }

    if (tokens[0] == "#" || tokens[0].empty()) {
      // comment or blank line — skip
    } else if (tokens[0] == "v" && tokens.size() >= 4) {
      auto x = std::stof(tokens[1]);
      auto y = std::stof(tokens[2]);
      auto z = std::stof(tokens[3]);
      tmp_positions.emplace_back(x, y, z);
    } else if (tokens[0] == "vn" && tokens.size() >= 4) {
      auto x = std::stof(tokens[1]);
      auto y = std::stof(tokens[2]);
      auto z = std::stof(tokens[3]);
      tmp_normals.emplace_back(x, y, z);
    } else if (tokens[0] == "vt" && tokens.size() >= 3) {
      auto u = std::stof(tokens[1]);
      auto v = std::stof(tokens[2]);
      tmp_texcoords.emplace_back(u, v);
    } else if (tokens[0] == "f" && tokens.size() >= 4) {
      // Parse face vertices. Each vertex spec can be:
      //   v
      //   v/vt
      //   v/vt/vn
      //   v//vn
      uint32_t face_indices[3];
      for (int i = 0; i < 3; ++i) {
        const auto& tok = tokens[i + 1];
        int pi = -1, ti = -1, ni = -1;

        auto slash1 = tok.find('/');
        if (slash1 == std::string::npos) {
          // format: v
          pi = std::stoi(tok) - 1;
        } else {
          pi = std::stoi(tok.substr(0, slash1)) - 1;
          auto slash2 = tok.find('/', slash1 + 1);
          if (slash2 == std::string::npos) {
            // format: v/vt
            ti = std::stoi(tok.substr(slash1 + 1)) - 1;
            has_texcoords = true;
          } else {
            // format: v/vt/vn or v//vn
            auto vt_str = tok.substr(slash1 + 1, slash2 - slash1 - 1);
            if (!vt_str.empty()) {
              ti = std::stoi(vt_str) - 1;
              has_texcoords = true;
            }
            ni = std::stoi(tok.substr(slash2 + 1)) - 1;
            has_normals = true;
          }
        }

        face_indices[i] = get_vertex_index(pi, ni, ti);
      }
      mesh->add_triangle(face_indices[0], face_indices[1], face_indices[2]);
    }

    index = newline_index(b, index + 1);
  }

  mesh->set_positions(std::move(out_positions));
  if (has_normals) {
    mesh->set_normals(std::move(out_normals));
  }
  if (has_texcoords) {
    mesh->set_texcoords0(std::move(out_texcoords));
  }

  std::cout << "Read " << mesh->triangle_count() << " triangles" << std::endl;

  auto node = std::make_shared<Node>();
  node->set_mesh(mesh);

  auto scene = std::make_shared<Scene>();
  scene->add_node(node);

  return scene;
}

// Returns the index of the first character following a group of newline characters after the offset
unsigned int ObjDecoder::newline_index(const std::vector<char>& b, int offset) {

  int ni = offset;

  while (ni < static_cast<int>(b.size()) && !(b[ni] == '\n' || b[ni] == '\r')) {
    ni++;
  }
  while (ni < static_cast<int>(b.size()) && (b[ni] == '\n' || b[ni] == '\r')) {
    ni++;
  }

  return ni;
}

// Returns true if c is a whitespace character
bool ObjDecoder::is_whitespace(char c) {
  return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

// Returns a collection of whitespace-separated character strings occuring between offset and the end of the
// line. Unlike the old version, '/' is NOT treated as a delimiter — it's kept as part of face vertex tokens.
std::vector<std::string> ObjDecoder::Tokenize(const std::vector<char>& b, int offset) {

  std::vector<std::string> tokens;

  auto end = newline_index(b, offset);
  // Back up past the newline chars to get the line end
  int line_end = end;
  while (line_end > offset && (b[line_end - 1] == '\n' || b[line_end - 1] == '\r')) {
    line_end--;
  }

  int pos = offset;
  // Skip leading whitespace
  while (pos < line_end && (b[pos] == ' ' || b[pos] == '\t')) {
    pos++;
  }

  while (pos < line_end) {
    int start = pos;
    while (pos < line_end && b[pos] != ' ' && b[pos] != '\t') {
      pos++;
    }
    tokens.emplace_back(&b[start], pos - start);
    // Skip whitespace between tokens
    while (pos < line_end && (b[pos] == ' ' || b[pos] == '\t')) {
      pos++;
    }
  }

  return tokens;
}
