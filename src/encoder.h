#ifndef TDC_ENCODER_H_
#define TDC_ENCODER_H_

#include "basetypes.h"
#include "model.h"

// Base encoder class that all encoders should extend
class Encoder {

 public:
  typedef boost::shared_ptr<Encoder> ShPtr;
 
  virtual bool encode(Model& m, std::string output_path) = 0;
  
};

#endif
