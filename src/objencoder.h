#ifndef TDC_OBJENCODER_H_
#define TDC_OBJENCODER_H_

#include <vector>

#include "basetypes.h"
#include "encoder.h"
#include "fileblob.h"
#include "model.h"

// Writes the contents of a Model to a binary STL file
class ObjEncoder : public Encoder {

 public:
  typedef boost::shared_ptr<ObjEncoder> ShPtr;
 
  ObjEncoder();
  ~ObjEncoder();
  bool encode(Model& m, std::string output_path);

 private:
  void WriteVerticies(std::ofstream& out, Triangle::ShPtr t);
  void WriteNormal(std::ofstream& out, Vertex::ShPtr v);
  void WriteFace(std::ofstream& out, int i);
   
  DISALLOW_COPY_AND_ASSIGN(ObjEncoder);
};

#endif
