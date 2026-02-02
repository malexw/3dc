#ifndef TDC_STLBENCODER_H_
#define TDC_STLBENCODER_H_

#include <vector>

#include "encoder.h"
#include "mesh.h"

// Writes the contents of a Scene to binary STL files
class StlbEncoder : public Encoder {

 public:
  typedef std::shared_ptr<StlbEncoder> ShPtr;

  StlbEncoder();
  ~StlbEncoder();
  bool encode(const Scene& scene, const std::filesystem::path& output_path) override;

 private:
  bool encode_mesh(const Mesh& m, const std::filesystem::path& output_path);
  void WriteStlbTriangle(std::ofstream& out, Triangle::ShPtr t);
  void WriteStlbVertex(std::ofstream& out, Vec3f::ShPtr v);

  StlbEncoder(const StlbEncoder&) = delete;
  StlbEncoder& operator=(const StlbEncoder&) = delete;
};

#endif
