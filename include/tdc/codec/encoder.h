#ifndef TDC_ENCODER_H_
#define TDC_ENCODER_H_

#include <filesystem>

#include "tdc/scene.h"

namespace tdc {

// Base encoder class that all encoders should extend. Turns Scenes into files.
class Encoder {

 public:
  typedef std::shared_ptr<Encoder> ShPtr;

  virtual ~Encoder() {}
  virtual bool encode(const Scene& scene, const std::filesystem::path& output_path) = 0;

};

}  // namespace tdc

#endif
