#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "decoder.h"
#include "encoder.h"
#include "scene.h"
#include "obj/objdecoder.h"
#include "obj/objencoder.h"
#include "stlb/stlbdecoder.h"
#include "stlb/stlbencoder.h"
#include "stl/stldecoder.h"
#include "stl/stlencoder.h"

void show_usage();

int main(int argc, char * argv[]) {

  if (argc == 5) {

    Decoder::ShPtr d;
    Encoder::ShPtr e;
    std::ifstream file(std::string(argv[2]), std::ios::binary | std::ios::ate);
    std::vector<char> b(file.tellg());
    file.seekg(0);
    file.read(b.data(), b.size());

    std::string_view input_fmt(argv[1]);
    std::string_view output_fmt(argv[3]);

    if (input_fmt == "obj") {
      d = std::make_shared<ObjDecoder>();
    } else if (input_fmt == "stl") {
      d = std::make_shared<StlDecoder>();
    } else if (input_fmt == "stlb") {
      d = std::make_shared<StlbDecoder>();
    } else {
      show_usage();
      return 0;
    }

    if (output_fmt == "obj") {
      e = std::make_shared<ObjEncoder>();
    } else if (output_fmt == "stl") {
      e = std::make_shared<StlEncoder>();
    } else if (output_fmt == "stlb") {
      e = std::make_shared<StlbEncoder>();
    } else {
      show_usage();
      return 0;
    }

    auto scene = d->decode(b);
    e->encode(*scene, argv[4]);

  } else {
    show_usage();
  }

  return 0;
}

void show_usage() {
  std::cout << "Error: invalid parameters" << std::endl;
  std::cout << "Usage: 3dc input_type input_file output_type output_file" << std::endl;
}
