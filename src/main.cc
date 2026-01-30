#include <iostream>
#include <string>

#include "decoder.h"
#include "encoder.h"
#include "fileblob.h"
#include "model.h"
#include "obj/objdecoder.h"
#include "obj/objencoder.h"
#include "stlb/stlbdecoder.h"
#include "stlb/stlbencoder.h"
#include "stl/stldecoder.h"
#include "stl/stlencoder.h"

//float byteArrayToFloat(const char * source, int len);
void show_usage();

int main(int argc, char * argv[]) {
    
  if (argc == 5) {
    
    Decoder::ShPtr d;
    Encoder::ShPtr e;
    FileBlob * b = new FileBlob(std::string(argv[2]));; 
    
    if (std::string(argv[1]) == "obj") {
	  d.reset(new ObjDecoder());
	} else if (std::string(argv[1]) == "stl") {
      d.reset(new StlDecoder());
    } else if (std::string(argv[1]) == "stlb") {
      d.reset(new StlbDecoder());
    } else {
      show_usage();
      return 0;
    }
	
	if (std::string(argv[3]) == "obj") {
	  e.reset(new ObjEncoder());
	} else if (std::string(argv[3]) == "stl") {
      e.reset(new StlEncoder());
    } else if (std::string(argv[3]) == "stlb") {
      e.reset(new StlbEncoder());
    } else {
      show_usage();
      return 0;
    }

    Model::ShPtr model = d->decode(*b);
    e->encode(*model, std::string(argv[4]));
    
  } else {
    show_usage();
  }

  return 0;
}

void show_usage() {
  std::cout << "Error: invalid parameters" << std::endl;
  std::cout << "Usage: 3dc input_type input_file output_type output_file" << std::endl;
}

// assuming little endian
/*float byteArrayToFloat(const char * source, int len) {
  int sign = source[3] & 0x80;
  sign >>= 7;
  //std::cout << "s3 " << source[3];
  int exp = source[3] & 0x7F;
  exp <<= 1;
  exp += source[2] & 80;
  exp -= 126;
  int val = (source[2] & 0x7F) + 0x80;
  val <<= 8;
  val += source[1];
  val <<= 8;
  val += source[0];
  //std::cout << "sign: " << sign << " exp: " << exp << " val: " << val << std::endl;
  float result = pow(-1, sign) * val / pow(2, 23) * pow(2, exp);
  return result;
}*/
