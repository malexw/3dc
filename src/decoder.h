#ifndef TDC_DECODER_H_
#define TDC_DECODER_H_

#include "fileblob.h"
#include "model.h"

// Base decoder class that all decoders should extend
class Decoder {

 public:
  typedef boost::shared_ptr<Decoder> ShPtr;
 
  virtual Model::ShPtr decode(FileBlob& b) = 0;

};

#endif
