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

  // Material models the output format can represent. Encoders for formats
  // with material support should override the ones they can express.
  virtual bool supports_blinn_phong_materials() const { return false; }
  virtual bool supports_pbr_materials() const { return false; }

 protected:
  // Convert any scene materials the output format can't represent to the
  // model it supports, warning about the lossy conversion. Encoders that
  // write materials should call this at the start of encode().
  void prepare_materials(const Scene& scene) const;

};

}  // namespace tdc

#endif
