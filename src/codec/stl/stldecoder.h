#ifndef TDC_STLDECODER_H_
#define TDC_STLDECODER_H_

#include <vector>

#include "basetypes.h"
#include "decoder.h"
#include "fileblob.h"
#include "model.h"

// Decoder class for reading ASCII STL files and converting them to Models
class StlDecoder : public Decoder {

 public:
  typedef std::shared_ptr<StlDecoder> ShPtr;
 
  StlDecoder();
  ~StlDecoder();
  Model::ShPtr decode(FileBlob& b);

 private:  
  // Returns the index of the first non-whitespace character after offset
  const unsigned int next_index(FileBlob& b, int offset);
  
  // Returns the index of the first character following a group of newline characters after the offset
  const unsigned int newline_index(FileBlob& b, int offset);
  
  // Returns true if c is a whitespace character
  const bool is_whitespace(char c);
  
  // Returns a collection of whitespace-separated character strings occuring between offset and the end of the
  // line
  std::vector<std::string> Tokenize(FileBlob& b, int offset);
  
  DISALLOW_COPY_AND_ASSIGN(StlDecoder);
};

#endif
