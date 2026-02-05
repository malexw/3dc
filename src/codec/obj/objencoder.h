#ifndef TDC_OBJENCODER_H_
#define TDC_OBJENCODER_H_

#include <filesystem>
#include <vector>

#include "encoder.h"
#include "material.h"
#include "mesh.h"

// Writes the contents of a Scene to OBJ files
class ObjEncoder : public Encoder {

 public:
  typedef std::shared_ptr<ObjEncoder> ShPtr;

  ObjEncoder();
  ~ObjEncoder();
  bool encode(const Scene& scene, const std::filesystem::path& output_path) override;

 private:
  // Write a .mtl file for all materials in the scene
  bool encode_mtl(const Scene& scene, const std::filesystem::path& mtl_path);

  // Write a Vec3f material property
  void write_vec3f(std::ostream& out, const std::string& keyword, const Vec3f& v);

  ObjEncoder(const ObjEncoder&) = delete;
  ObjEncoder& operator=(const ObjEncoder&) = delete;
};

#endif
