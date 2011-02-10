#ifndef TDC_STLBDECODER_H_
#define TDC_STLBDECODER_H_

#include <vector>

#include "basetypes.h"
#include "decoder.h"
#include "fileblob.h"
#include "model.h"

// Decoder class for reading binary STL files
class StlbDecoder : public Decoder {

 public:
  typedef boost::shared_ptr<StlbDecoder> ShPtr;
  
  StlbDecoder();
  ~StlbDecoder();
  Model::ShPtr decode(FileBlob& b);

 private:
  float btof(const char* start);
  
  DISALLOW_COPY_AND_ASSIGN(StlbDecoder);
};

#endif
