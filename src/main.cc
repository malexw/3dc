#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "decoder.h"
#include "encoder.h"
#include "material_convert.h"
#include "scene.h"
#include "transform.h"
#include "obj/objdecoder.h"
#include "obj/objencoder.h"
#include "stlb/stlbdecoder.h"
#include "stlb/stlbencoder.h"
#include "stl/stldecoder.h"
#include "stl/stlencoder.h"

void show_usage();
bool is_identity(const transform::Mat4& m);

int main(int argc, char * argv[]) {

  // Parse command-line arguments
  transform::Mat4 xform = transform::identity();
  std::string material_mode;  // "pbr" or "blinn-phong", empty = no conversion
  std::vector<std::string_view> positional;

  int i = 1;
  while (i < argc) {
    std::string_view arg(argv[i]);

    if (arg == "-t" || arg == "--translate") {
      if (i + 3 >= argc) {
        std::cerr << "Error: --translate requires 3 arguments (X Y Z)" << std::endl;
        return 1;
      }
      float x = std::strtof(argv[i + 1], nullptr);
      float y = std::strtof(argv[i + 2], nullptr);
      float z = std::strtof(argv[i + 3], nullptr);
      xform = transform::multiply(transform::translation(x, y, z), xform);
      i += 4;
    } else if (arg == "-rx" || arg == "--rotate-x") {
      if (i + 1 >= argc) {
        std::cerr << "Error: --rotate-x requires 1 argument (degrees)" << std::endl;
        return 1;
      }
      float deg = std::strtof(argv[i + 1], nullptr);
      xform = transform::multiply(transform::rotation_x(deg), xform);
      i += 2;
    } else if (arg == "-ry" || arg == "--rotate-y") {
      if (i + 1 >= argc) {
        std::cerr << "Error: --rotate-y requires 1 argument (degrees)" << std::endl;
        return 1;
      }
      float deg = std::strtof(argv[i + 1], nullptr);
      xform = transform::multiply(transform::rotation_y(deg), xform);
      i += 2;
    } else if (arg == "-rz" || arg == "--rotate-z") {
      if (i + 1 >= argc) {
        std::cerr << "Error: --rotate-z requires 1 argument (degrees)" << std::endl;
        return 1;
      }
      float deg = std::strtof(argv[i + 1], nullptr);
      xform = transform::multiply(transform::rotation_z(deg), xform);
      i += 2;
    } else if (arg == "-s" || arg == "--scale") {
      if (i + 1 >= argc) {
        std::cerr << "Error: --scale requires 1 argument (factor)" << std::endl;
        return 1;
      }
      float factor = std::strtof(argv[i + 1], nullptr);
      xform = transform::multiply(transform::scale(factor), xform);
      i += 2;
    } else if (arg == "-m" || arg == "--materials") {
      if (i + 1 >= argc) {
        std::cerr << "Error: --materials requires 1 argument (pbr or blinn-phong)" << std::endl;
        return 1;
      }
      material_mode = argv[i + 1];
      if (material_mode != "pbr" && material_mode != "blinn-phong") {
        std::cerr << "Error: --materials mode must be 'pbr' or 'blinn-phong'" << std::endl;
        return 1;
      }
      i += 2;
    } else {
      positional.push_back(arg);
      i++;
    }
  }

  if (positional.size() != 4) {
    show_usage();
    return 1;
  }

  Decoder::ShPtr d;
  Encoder::ShPtr e;

  std::string input_file(positional[1]);
  std::ifstream file(input_file, std::ios::binary | std::ios::ate);
  if (!file) {
    std::cerr << "Error: could not open input file: " << input_file << std::endl;
    return 1;
  }
  std::vector<char> b(file.tellg());
  file.seekg(0);
  file.read(b.data(), b.size());

  std::string_view input_fmt = positional[0];
  std::string_view output_fmt = positional[2];

  if (input_fmt == "obj") {
    d = std::make_shared<ObjDecoder>();
  } else if (input_fmt == "stl") {
    d = std::make_shared<StlDecoder>();
  } else if (input_fmt == "stlb") {
    d = std::make_shared<StlbDecoder>();
  } else {
    std::cerr << "Error: unknown input format: " << input_fmt << std::endl;
    show_usage();
    return 1;
  }

  if (output_fmt == "obj") {
    e = std::make_shared<ObjEncoder>();
  } else if (output_fmt == "stl") {
    e = std::make_shared<StlEncoder>();
  } else if (output_fmt == "stlb") {
    e = std::make_shared<StlbEncoder>();
  } else {
    std::cerr << "Error: unknown output format: " << output_fmt << std::endl;
    show_usage();
    return 1;
  }

  auto scene = d->decode(b, input_file.c_str());

  // Convert materials if requested
  if (!material_mode.empty()) {
    for (const auto& mat : scene->materials()) {
      if (material_mode == "pbr") {
        convert_to_pbr(*mat);
      } else {
        convert_to_blinn_phong(*mat);
      }
    }
  }

  // Apply transform if not identity
  if (!is_identity(xform)) {
    for (const auto& node : scene->nodes()) {
      if (node->mesh()) {
        node->mesh()->apply_transform(xform);
      }
    }
  }

  std::string output_file(positional[3]);
  e->encode(*scene, output_file.c_str());

  return 0;
}

bool is_identity(const transform::Mat4& m) {
  const transform::Mat4 identity = transform::identity();
  for (int i = 0; i < 16; ++i) {
    if (m[i] != identity[i]) return false;
  }
  return true;
}

void show_usage() {
  std::cout << "Usage: 3dc [OPTIONS] input_type input_file output_type output_file" << std::endl;
  std::cout << std::endl;
  std::cout << "Transform options (applied in order specified):" << std::endl;
  std::cout << "  -t, --translate X Y Z    Translate by (X, Y, Z)" << std::endl;
  std::cout << "  -rx, --rotate-x DEG      Rotate around X axis by DEG degrees" << std::endl;
  std::cout << "  -ry, --rotate-y DEG      Rotate around Y axis by DEG degrees" << std::endl;
  std::cout << "  -rz, --rotate-z DEG      Rotate around Z axis by DEG degrees" << std::endl;
  std::cout << "  -s, --scale FACTOR       Uniform scale by FACTOR" << std::endl;
  std::cout << std::endl;
  std::cout << "Material options:" << std::endl;
  std::cout << "  -m, --materials MODE     Convert materials to MODE (pbr, blinn-phong)" << std::endl;
  std::cout << std::endl;
  std::cout << "Supported formats: stl, stlb, obj" << std::endl;
}
