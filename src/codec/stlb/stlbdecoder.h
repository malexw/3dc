#ifndef TDC_STLBDECODER_H_
#define TDC_STLBDECODER_H_

#include <vector>

#include "decoder.h"
#include "scene.h"

// Decoder class for reading binary STL files
class StlbDecoder : public Decoder {

 public:
  typedef std::shared_ptr<StlbDecoder> ShPtr;

  StlbDecoder();
  ~StlbDecoder();
  Scene::ShPtr decode(const std::vector<char>& b,
                      const std::filesystem::path& source_path) override;

 private:
  float btof(const char* start);

  StlbDecoder(const StlbDecoder&) = delete;
  StlbDecoder& operator=(const StlbDecoder&) = delete;
};

#endif
