#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>

#include "objdecoder.h"
#include "material.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "triangulate.h"
#include "vec2f.h"
#include "vec3f.h"

ObjDecoder::ObjDecoder() {
}

ObjDecoder::~ObjDecoder() {
}

Scene::ShPtr ObjDecoder::decode(const std::vector<char>& b,
                                const std::filesystem::path& source_path) {

  int index = 0;

  // Temporary arrays indexed by OBJ's 1-based indices (stored 0-based here)
  std::vector<Vec3f> tmp_positions;
  std::vector<Vec3f> tmp_normals;
  std::vector<Vec2f> tmp_texcoords;

  // Material library: name -> Material
  std::map<std::string, Material::ShPtr> materials;

  // Per-material mesh building state
  struct MeshBuilder {
    std::shared_ptr<Mesh> mesh;
    std::vector<Vec3f> out_positions;
    std::vector<Vec3f> out_normals;
    std::vector<Vec2f> out_texcoords;
    std::map<std::tuple<int, int, int>, uint32_t> vertex_map;
    bool has_normals = false;
    bool has_texcoords = false;
  };

  // Mesh builders keyed by material name ("" = no material)
  std::map<std::string, MeshBuilder> builders;
  // Track insertion order so output is deterministic
  std::vector<std::string> builder_order;

  std::string current_material;

  auto get_builder = [&](const std::string& mat_name) -> MeshBuilder& {
    auto it = builders.find(mat_name);
    if (it != builders.end()) return it->second;
    auto& mb = builders[mat_name];
    mb.mesh = std::make_shared<Mesh>();
    builder_order.push_back(mat_name);
    return mb;
  };

  while (index < static_cast<int>(b.size())) {
    auto tokens = Tokenize(b, index);

    if (tokens.empty()) {
      break;
    }

    if (tokens[0] == "#" || tokens[0].empty()) {
      // comment or blank line — skip
    } else if (tokens[0] == "mtllib" && tokens.size() >= 2) {
      // Load material library file
      auto mtl_path = source_path.parent_path() / tokens[1];
      std::ifstream mtl_file(mtl_path, std::ios::binary | std::ios::ate);
      if (mtl_file.is_open()) {
        std::vector<char> mtl_buf(mtl_file.tellg());
        mtl_file.seekg(0);
        mtl_file.read(mtl_buf.data(), mtl_buf.size());
        materials = parse_mtl(mtl_buf);
      } else {
        std::cout << "Warning: could not open material library: "
                  << mtl_path.string() << std::endl;
      }
    } else if (tokens[0] == "usemtl" && tokens.size() >= 2) {
      current_material = tokens[1];
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
      auto& mb = get_builder(current_material);

      // Helper to get or create a unified vertex index within this builder
      auto get_vertex_index = [&](int pi, int ni, int ti) -> uint32_t {
        auto key = std::make_tuple(pi, ni, ti);
        auto it = mb.vertex_map.find(key);
        if (it != mb.vertex_map.end()) {
          return it->second;
        }
        uint32_t idx = mb.out_positions.size();
        mb.out_positions.push_back(tmp_positions[pi]);
        if (ni >= 0) {
          mb.out_normals.push_back(tmp_normals[ni]);
          mb.has_normals = true;
        }
        if (ti >= 0) {
          mb.out_texcoords.push_back(tmp_texcoords[ti]);
          mb.has_texcoords = true;
        }
        mb.vertex_map[key] = idx;
        return idx;
      };

      // Parse face vertices. Each vertex spec can be:
      //   v
      //   v/vt
      //   v/vt/vn
      //   v//vn
      int face_verts = tokens.size() - 1;
      std::vector<uint32_t> face_indices(face_verts);
      std::vector<int> face_pos_indices(face_verts);
      for (int i = 0; i < face_verts; ++i) {
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
          } else {
            // format: v/vt/vn or v//vn
            auto vt_str = tok.substr(slash1 + 1, slash2 - slash1 - 1);
            if (!vt_str.empty()) {
              ti = std::stoi(vt_str) - 1;
            }
            ni = std::stoi(tok.substr(slash2 + 1)) - 1;
          }
        }

        face_pos_indices[i] = pi;
        face_indices[i] = get_vertex_index(pi, ni, ti);
      }

      if (face_verts == 3) {
        mb.mesh->add_triangle(face_indices[0], face_indices[1], face_indices[2]);
      } else {
        // Ear clipping triangulation for 4+ vertex polygons.
        auto tris = triangulate_polygon(tmp_positions, face_pos_indices);
        for (const auto& tri : tris) {
          mb.mesh->add_triangle(face_indices[tri[0]],
                                face_indices[tri[1]],
                                face_indices[tri[2]]);
        }
      }
    }

    index = newline_index(b, index + 1);
  }

  // Build scene from mesh builders
  auto scene = std::make_shared<Scene>();

  // Add all referenced materials to the scene
  std::set<std::string> added_materials;
  for (const auto& mat_name : builder_order) {
    if (!mat_name.empty() && added_materials.find(mat_name) == added_materials.end()) {
      auto it = materials.find(mat_name);
      if (it != materials.end()) {
        scene->add_material(it->second);
        added_materials.insert(mat_name);
      }
    }
  }

  int total_triangles = 0;
  for (const auto& mat_name : builder_order) {
    auto& mb = builders[mat_name];
    mb.mesh->set_positions(std::move(mb.out_positions));
    if (mb.has_normals) {
      mb.mesh->set_normals(std::move(mb.out_normals));
    }
    if (mb.has_texcoords) {
      mb.mesh->set_texcoords0(std::move(mb.out_texcoords));
    }

    // Assign material to mesh
    if (!mat_name.empty()) {
      auto it = materials.find(mat_name);
      if (it != materials.end()) {
        mb.mesh->set_material(it->second);
      }
    }

    total_triangles += mb.mesh->triangle_count();

    auto node = std::make_shared<Node>();
    node->set_mesh(mb.mesh);
    scene->add_node(node);
  }

  std::cout << "Read " << total_triangles << " triangles" << std::endl;

  return scene;
}

std::map<std::string, Material::ShPtr> ObjDecoder::parse_mtl(
    const std::vector<char>& b) {

  std::map<std::string, Material::ShPtr> materials;
  Material::ShPtr current;
  int index = 0;

  while (index < static_cast<int>(b.size())) {
    auto tokens = Tokenize(b, index);

    if (tokens.empty()) {
      break;
    }

    if (tokens[0] == "#" || tokens[0].empty()) {
      // comment or blank line
    } else if (tokens[0] == "newmtl" && tokens.size() >= 2) {
      current = std::make_shared<Material>();
      current->set_name(tokens[1]);
      materials[tokens[1]] = current;
    } else if (!current) {
      // No active material — skip
    } else if (tokens[0] == "Ka" && tokens.size() >= 4) {
      current->set_ka(Vec3f(std::stof(tokens[1]), std::stof(tokens[2]),
                            std::stof(tokens[3])));
    } else if (tokens[0] == "Kd" && tokens.size() >= 4) {
      current->set_kd(Vec3f(std::stof(tokens[1]), std::stof(tokens[2]),
                            std::stof(tokens[3])));
    } else if (tokens[0] == "Ks" && tokens.size() >= 4) {
      current->set_ks(Vec3f(std::stof(tokens[1]), std::stof(tokens[2]),
                            std::stof(tokens[3])));
    } else if (tokens[0] == "Ns" && tokens.size() >= 2) {
      current->set_ns(std::stof(tokens[1]));
    } else if (tokens[0] == "d" && tokens.size() >= 2) {
      current->set_d(std::stof(tokens[1]));
    } else if (tokens[0] == "Tr" && tokens.size() >= 2) {
      // Tr is transparency (inverse of dissolve)
      current->set_d(1.0f - std::stof(tokens[1]));
    } else if (tokens[0] == "illum" && tokens.size() >= 2) {
      current->set_illum(std::stoi(tokens[1]));
    } else if (tokens[0] == "map_Ka" && tokens.size() >= 2) {
      current->set_map_ka(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "map_Ka"));
    } else if (tokens[0] == "map_Kd" && tokens.size() >= 2) {
      current->set_map_kd(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "map_Kd"));
    } else if (tokens[0] == "map_Ks" && tokens.size() >= 2) {
      current->set_map_ks(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "map_Ks"));
    } else if (tokens[0] == "map_d" && tokens.size() >= 2) {
      current->set_map_d(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "map_d"));
    } else if ((tokens[0] == "map_bump" || tokens[0] == "bump") &&
               tokens.size() >= 2) {
      current->set_map_bump(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, tokens[0]));
    } else if (tokens[0] == "disp" && tokens.size() >= 2) {
      current->set_disp(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "disp"));
    } else if (tokens[0] == "decal" && tokens.size() >= 2) {
      current->set_decal(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "decal"));
    } else if (tokens[0] == "Pr" && tokens.size() >= 2) {
      current->set_pr(std::stof(tokens[1]));
    } else if (tokens[0] == "Pm" && tokens.size() >= 2) {
      current->set_pm(std::stof(tokens[1]));
    } else if (tokens[0] == "Ke" && tokens.size() >= 4) {
      current->set_ke(Vec3f(std::stof(tokens[1]), std::stof(tokens[2]),
                            std::stof(tokens[3])));
    } else if (tokens[0] == "map_Pr" && tokens.size() >= 2) {
      current->set_map_pr(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "map_Pr"));
    } else if (tokens[0] == "map_Pm" && tokens.size() >= 2) {
      current->set_map_pm(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "map_Pm"));
    } else if (tokens[0] == "map_Ke" && tokens.size() >= 2) {
      current->set_map_ke(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "map_Ke"));
    } else if (tokens[0] == "norm" && tokens.size() >= 2) {
      current->set_norm(parse_texture_map(
          {tokens.begin() + 1, tokens.end()}, "norm"));
    } else {
      std::cout << "Warning: unsupported material property: " << tokens[0]
                << std::endl;
    }

    index = newline_index(b, index + 1);
  }

  return materials;
}

std::string ObjDecoder::parse_texture_map(
    const std::vector<std::string>& tokens, const std::string& keyword) {

  if (tokens.empty()) return "";

  // Check for texture options (tokens starting with '-')
  bool has_options = false;
  for (size_t i = 0; i + 1 < tokens.size(); ++i) {
    if (!tokens[i].empty() && tokens[i][0] == '-') {
      has_options = true;
      break;
    }
  }

  if (has_options) {
    std::cout << "Warning: ignoring texture options for " << keyword
              << std::endl;
  }

  // The filename is the last token
  return tokens.back();
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
