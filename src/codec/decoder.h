#ifndef TDC_DECODER_H_
#define TDC_DECODER_H_

#include <filesystem>
#include <vector>

#include "scene.h"

// Base decoder class that all decoders should extend. Responsible for turning input files into Scene objects.
// source_path is the path to the input file, used to resolve references to auxiliary files (e.g. MTL).
class Decoder {

 public:
  typedef std::shared_ptr<Decoder> ShPtr;

  virtual ~Decoder() {}
  virtual Scene::ShPtr decode(const std::vector<char>& b,
                              const std::filesystem::path& source_path) = 0;

};

#endif
