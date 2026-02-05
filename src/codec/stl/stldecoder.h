#ifndef TDC_STLDECODER_H_
#define TDC_STLDECODER_H_

#include <vector>

#include "decoder.h"
#include "scene.h"

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

  // Returns the index of the first character following a group of newline characters after the offset
  unsigned int newline_index(const std::vector<char>& b, int offset);

  // Returns true if c is a whitespace character
  bool is_whitespace(char c);

  // Returns a collection of whitespace-separated character strings occuring between offset and the end of the
  // line
  std::vector<std::string> Tokenize(const std::vector<char>& b, int offset);

  StlDecoder(const StlDecoder&) = delete;
  StlDecoder& operator=(const StlDecoder&) = delete;
};

#endif
