#ifndef TDC_STLDECODER_H_
#define TDC_STLDECODER_H_

#include <vector>

#include "tdc/codec/decoder.h"
#include "tdc/scene.h"

namespace tdc {

// Decoder class for reading ASCII STL files and converting them to Scenes
class StlDecoder : public Decoder {

 public:
  typedef std::shared_ptr<StlDecoder> ShPtr;

  StlDecoder();
  ~StlDecoder();
  Scene::ShPtr decode(const std::vector<char>& b,
                      const std::filesystem::path& source_path) override;

 private:
  // Returns the index of the first non-whitespace character after offset
  unsigned int next_index(const std::vector<char>& b, int offset);

  // Returns the index of the start of the next non-empty line
  unsigned int newline_index(const std::vector<char>& b, int offset);

  bool is_whitespace(char c);

  // Splits the line at offset into whitespace-separated tokens
  std::vector<std::string> Tokenize(const std::vector<char>& b, int offset);

  StlDecoder(const StlDecoder&) = delete;
  StlDecoder& operator=(const StlDecoder&) = delete;
};

}  // namespace tdc

#endif
