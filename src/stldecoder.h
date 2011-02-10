#ifndef TDC_STLDECODER_H_
#define TDC_STLDECODER_H_

#include <vector>

#include "basetypes.h"
#include "decoder.h"
#include "fileblob.h"
#include "model.h"

// Decoder class for reading ASCII STL files
class StlDecoder : public Decoder {

 public:
  typedef boost::shared_ptr<StlDecoder> ShPtr;
 
  StlDecoder();
  ~StlDecoder();
  Model::ShPtr decode(FileBlob& b);

 private:
  int is_next_token(FileBlob& b, std::string token, int offset);
  unsigned int next_index(FileBlob& b, int offset);
  unsigned int newline_index(FileBlob& b, int offset);
  bool is_whitespace(char c);
  std::vector<std::string> Tokenize(FileBlob& b, int offset);
  
  DISALLOW_COPY_AND_ASSIGN(StlDecoder);
};

#endif
