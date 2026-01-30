#ifndef TDC_STLENCODER_H_
#define TDC_STLENCODER_H_

#include <vector>

#include "basetypes.h"
#include "encoder.h"
#include "fileblob.h"
#include "model.h"

// Writes the contents of a Model to a binary STL file
class StlEncoder : public Encoder {

 public:
  typedef std::shared_ptr<StlEncoder> ShPtr;
 
  StlEncoder();
  ~StlEncoder();
  bool encode(Model& m, std::string output_path);

 private:
  void WriteStlTriangle(std::ofstream& out, Triangle::ShPtr t);
  void WriteStlVertex(std::ofstream& out, Vertex::ShPtr v);
   
  DISALLOW_COPY_AND_ASSIGN(StlEncoder);
};

#endif
