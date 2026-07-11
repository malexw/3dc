#ifndef TDC_OBJENCODER_H_
#define TDC_OBJENCODER_H_

#include <filesystem>
#include <vector>

#include "tdc/codec/encoder.h"
#include "tdc/material.h"
#include "tdc/mesh.h"

namespace tdc {

// Writes the contents of a Scene to OBJ files
class ObjEncoder : public Encoder {

 public:
  typedef std::shared_ptr<ObjEncoder> ShPtr;

  ObjEncoder();
  ~ObjEncoder();
  bool encode(const Scene& scene, const std::filesystem::path& output_path) override;

  // MTL can express both Blinn-Phong and PBR (extension) properties.
  bool supports_blinn_phong_materials() const override { return true; }
  bool supports_pbr_materials() const override { return true; }

 private:
  bool encode_mtl(const Scene& scene, const std::filesystem::path& mtl_path);

  void write_vec3f(std::ostream& out, const std::string& keyword, const Vec3f& v);

  ObjEncoder(const ObjEncoder&) = delete;
  ObjEncoder& operator=(const ObjEncoder&) = delete;
};

}  // namespace tdc

#endif
