#ifndef TDC_STLBENCODER_H_
#define TDC_STLBENCODER_H_

#include <vector>

#include "basetypes.h"
#include "fileblob.h"
#include "model.h"

// Writes the contents of a Model to a binary STL file
class StlbEncoder : public Encoder {

 public:
  typedef std::shared_ptr<StlbEncoder> ShPtr;
  
  StlbEncoder();
  ~StlbEncoder();
  bool encode(Model& m, std::string output_path);

 private:
  void WriteStlbTriangle(std::ofstream& out, Triangle::ShPtr t);
  void WriteStlbVertex(std::ofstream& out, Vertex::ShPtr v);
   
  DISALLOW_COPY_AND_ASSIGN(StlbEncoder);
};

#endif
