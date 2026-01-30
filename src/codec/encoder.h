#ifndef TDC_ENCODER_H_
#define TDC_ENCODER_H_

#include "basetypes.h"
#include "model.h"

// Base encoder class that all encoders should extend. Turns Models into files.
class Encoder {

 public:
  typedef std::shared_ptr<Encoder> ShPtr;
 
  virtual bool encode(Model& m, std::string output_path) = 0;
  
};

#endif
