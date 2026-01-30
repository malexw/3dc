#ifndef TDC_OBJDECODER_H_
#define TDC_OBJDECODER_H_

#include <vector>

#include "basetypes.h"
#include "decoder.h"
#include "fileblob.h"
#include "model.h"

// Decoder class for reading ASCII OBJ files
class ObjDecoder : public Decoder {

 public:
  typedef std::shared_ptr<ObjDecoder> ShPtr;
 
  ObjDecoder();
  ~ObjDecoder();
  Model::ShPtr decode(FileBlob& b);

 private:
  // Returns the index of the first non-whitespace character after offset
  unsigned int next_index(FileBlob& b, int offset);
  // Returns the index of the first character following a group of newline characters after the offset
  unsigned int newline_index(FileBlob& b, int offset);
  // Returns true if c is a whitespace character
  bool is_whitespace(char c);
  // Returns a collection of whitespace-separated character strings occuring between offset and the end of the
  // line
  std::vector<std::string> Tokenize(FileBlob& b, int offset);
  
  DISALLOW_COPY_AND_ASSIGN(ObjDecoder);
};

#endif
