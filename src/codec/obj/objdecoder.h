#ifndef TDC_OBJDECODER_H_
#define TDC_OBJDECODER_H_

#include <vector>

#include "decoder.h"
#include "scene.h"

// Decoder class for reading ASCII OBJ files
class ObjDecoder : public Decoder {

 public:
  typedef std::shared_ptr<ObjDecoder> ShPtr;

  ObjDecoder();
  ~ObjDecoder();
  Scene::ShPtr decode(const std::vector<char>& b) override;

 private:
  // Returns the index of the first character following a group of newline characters after the offset
  unsigned int newline_index(const std::vector<char>& b, int offset);
  // Returns true if c is a whitespace character
  bool is_whitespace(char c);
  // Returns a collection of whitespace-separated character strings occuring between offset and the end of the
  // line
  std::vector<std::string> Tokenize(const std::vector<char>& b, int offset);

  ObjDecoder(const ObjDecoder&) = delete;
  ObjDecoder& operator=(const ObjDecoder&) = delete;
};

#endif
