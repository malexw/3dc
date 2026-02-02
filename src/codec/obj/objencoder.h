#ifndef TDC_OBJENCODER_H_
#define TDC_OBJENCODER_H_

#include <vector>

#include "encoder.h"
#include "mesh.h"

// Writes the contents of a Scene to OBJ files
class ObjEncoder : public Encoder {

 public:
  typedef std::shared_ptr<ObjEncoder> ShPtr;

  ObjEncoder();
  ~ObjEncoder();
  bool encode(const Scene& scene, const std::filesystem::path& output_path) override;

 private:
  bool encode_mesh(const Mesh& m, const std::filesystem::path& output_path);
  void WriteVerticies(std::ofstream& out, Triangle::ShPtr t);
  void WriteNormal(std::ofstream& out, Vec3f::ShPtr v);
  void WriteFace(std::ofstream& out, int i);

  ObjEncoder(const ObjEncoder&) = delete;
  ObjEncoder& operator=(const ObjEncoder&) = delete;
};

#endif
