#include <cstdint>
#include <cstring>
#include <iostream>

#include "stlbdecoder.h"
#include "mesh.h"
#include "node.h"
#include "scene.h"
#include "triangle.h"
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

  for (i = 0x54; i < fsize; i += 0x32) {
    auto normal = std::make_shared<Vec3f>(btof(&b[i]), btof(&b[i+0x4]), btof(&b[i+0x8]));
    auto v1 = std::make_shared<Vec3f>(btof(&b[i+0xC]), btof(&b[i+0x10]), btof(&b[i+0x14]));
    auto v2 = std::make_shared<Vec3f>(btof(&b[i+0x18]), btof(&b[i+0x1C]), btof(&b[i+0x20]));
    auto v3 = std::make_shared<Vec3f>(btof(&b[i+0x24]), btof(&b[i+0x28]), btof(&b[i+0x2C]));

    auto tri = std::make_shared<Triangle>(v1, v2, v3, normal);
    mesh->add_triangle(tri);
  }

  if (triangle_count == mesh->triangle_count() ) {
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
