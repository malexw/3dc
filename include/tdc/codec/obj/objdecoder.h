#ifndef TDC_OBJDECODER_H_
#define TDC_OBJDECODER_H_

#include <map>
#include <string>
#include <vector>

#include "tdc/codec/decoder.h"
#include "tdc/material.h"
#include "tdc/scene.h"

namespace tdc {

// Decoder class for reading ASCII OBJ files
class ObjDecoder : public Decoder {

 public:
  typedef std::shared_ptr<ObjDecoder> ShPtr;

  ObjDecoder();
  ~ObjDecoder();
  Scene::ShPtr decode(const std::vector<char>& b,
                      const std::filesystem::path& source_path) override;

 private:
  // Parse an MTL file buffer and return a map of material name -> Material
  std::map<std::string, Material::ShPtr> parse_mtl(const std::vector<char>& b);

  // Parse a texture map line, extracting the filename and warning about ignored options.
  // tokens contains the tokens after the keyword (e.g. after "map_Kd").
  std::string parse_texture_map(const std::vector<std::string>& tokens,
                                const std::string& keyword);

  // Returns the index of the start of the next non-empty line
  unsigned int newline_index(const std::vector<char>& b, int offset);
  bool is_whitespace(char c);
  // Splits the line at offset into whitespace-separated tokens. '/' is not a
  // delimiter, so face vertex specs like "1/2/3" stay intact.
  std::vector<std::string> Tokenize(const std::vector<char>& b, int offset);

  ObjDecoder(const ObjDecoder&) = delete;
  ObjDecoder& operator=(const ObjDecoder&) = delete;
};

}  // namespace tdc

#endif
