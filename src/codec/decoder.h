#ifndef TDC_DECODER_H_
#define TDC_DECODER_H_

#include <vector>

#include "scene.h"

// Base decoder class that all decoders should extend. Responsible for turning input files into Scene objects.
class Decoder {

 public:
  typedef std::shared_ptr<Decoder> ShPtr;

  virtual ~Decoder() {}
  virtual Scene::ShPtr decode(const std::vector<char>& b) = 0;

};

#endif
