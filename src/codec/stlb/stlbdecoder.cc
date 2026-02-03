#include <cstdint>
#include <cstring>
#include <iostream>

#include "stlbdecoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "vec3f.h"

StlbDecoder::StlbDecoder() {
}

StlbDecoder::~StlbDecoder() {
}

Scene::ShPtr StlbDecoder::decode(const std::vector<char>& b) {

  auto mesh = std::make_shared<Mesh>();
  auto fsize = b.size();

  unsigned int i = 0, triangle_count = 0;
  for ( ; i < 4; ++i) {
    triangle_count <<= 8;
    triangle_count += static_cast<uint8_t>(b[0x53-i]);
  }

  std::vector<Vec3f> positions;
  std::vector<Vec3f> normals;
  uint32_t vert_index = 0;

  for (i = 0x54; i < fsize; i += 0x32) {
    Vec3f normal(btof(&b[i]), btof(&b[i+0x4]), btof(&b[i+0x8]));
    Vec3f v1(btof(&b[i+0xC]), btof(&b[i+0x10]), btof(&b[i+0x14]));
    Vec3f v2(btof(&b[i+0x18]), btof(&b[i+0x1C]), btof(&b[i+0x20]));
    Vec3f v3(btof(&b[i+0x24]), btof(&b[i+0x28]), btof(&b[i+0x2C]));

    positions.push_back(v1);
    positions.push_back(v2);
    positions.push_back(v3);

    // Expand face normal to per-vertex
    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);

    mesh->add_triangle(vert_index, vert_index + 1, vert_index + 2);
    vert_index += 3;
  }

  mesh->set_positions(std::move(positions));
  mesh->set_normals(std::move(normals));

  if (triangle_count == mesh->triangle_count()) {
    std::cout << "Read " << triangle_count << " triangles" << std::endl;
  } else {
    std::cout << "Error: triangle count mismatch. Expected " << triangle_count;
    std::cout << " got " << mesh->triangle_count() << std::endl;
  }

  auto node = std::make_shared<Node>();
  node->set_mesh(mesh);

  auto scene = std::make_shared<Scene>();
  scene->add_node(node);

  return scene;
}

float StlbDecoder::btof(const char* start) {

  uint32_t bits =
    (static_cast<uint8_t>(start[0]))       |
    (static_cast<uint8_t>(start[1]) << 8)  |
    (static_cast<uint8_t>(start[2]) << 16) |
    (static_cast<uint8_t>(start[3]) << 24);
  float ret;
  std::memcpy(&ret, &bits, sizeof(ret));

  return ret;
}
