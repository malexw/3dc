#ifndef TDC_STLENCODER_H_
#define TDC_STLENCODER_H_

#include <vector>

#include "tdc/codec/encoder.h"
#include "tdc/mesh.h"

namespace tdc {

// Writes the contents of a Scene to ASCII STL files
class StlEncoder : public Encoder {

 public:
  typedef std::shared_ptr<StlEncoder> ShPtr;

  StlEncoder();
  ~StlEncoder();
  bool encode(const Scene& scene, const std::filesystem::path& output_path) override;

 private:
  bool encode_mesh(const Mesh& m, const std::filesystem::path& output_path);

  StlEncoder(const StlEncoder&) = delete;
  StlEncoder& operator=(const StlEncoder&) = delete;
};

}  // namespace tdc

#endif
