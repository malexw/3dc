#include <boost/lexical_cast.hpp>
#include <iostream>

#include "stldecoder.h"
#include "model.h"
#include "triangle.h"
#include "vertex.h"

StlDecoder::StlDecoder() {
}

StlDecoder::~StlDecoder() {
}

Model::ShPtr StlDecoder::decode(FileBlob& b) {
  
  std::string header = "solid";
  std::string facet = "facet";
  std::string normal = "normal";
  std::string outer = "outer";
  std::string loop = "loop";
  std::string vertex = "vertex";
  std::string endloop = "endloop";
  std::string endfacet = "endfacet";
  std::string endsolid = "endsolid";
  
  int index = 0;
  int triangles = 0;
  
  std::vector<Vertex::ShPtr> verts;
  Model::ShPtr model (new Model());

  std::vector<std::string> tokens = Tokenize(b, index);
    
  if (tokens[0].compare(header)) {
    std::cout << "Error: missing header" << std::endl;
    return model;
  }
   
  index = newline_index(b, index+1);
  tokens = Tokenize(b, index);
  
  while (tokens[0].compare(endsolid)) {
    
    if (!(tokens[0].compare(facet))) {
      if (!(tokens[1].compare(normal))) {
        float x = boost::lexical_cast<float>(tokens[2]);
        float y = boost::lexical_cast<float>(tokens[3]);
        float z = boost::lexical_cast<float>(tokens[4]);
        Vertex::ShPtr v (new Vertex(x, y, z));
        verts.push_back(v);
      } else {
        std::cout << "Error: missing 'normal'" << std::endl;
        return model;
      }
    } else {
      std::cout << "Error: missing 'facet'" << std::endl;
      return model;
    }
    
    index = newline_index(b, index+1);
    tokens = Tokenize(b, index);
    
    if (tokens[0].compare(outer) || tokens[1].compare(loop)) {
      std::cout << "Error: missing 'outer loop'" << std::endl;
      return model;
    }
    
    for (int i = 3; i > 0; --i) {
      index = newline_index(b, index+1);
      tokens = Tokenize(b, index);
      
      if (!(tokens[0].compare(vertex))) {
        float x = boost::lexical_cast<float>(tokens[1]);
        float y = boost::lexical_cast<float>(tokens[2]);
        float z = boost::lexical_cast<float>(tokens[3]);
        Vertex::ShPtr v (new Vertex(x, y, z));
        verts.push_back(v);
      } else {
        std::cout << "Error: missing 'vertex'" << std::endl;
        return model;
      }
    }
    
    index = newline_index(b, index+1);
    tokens = Tokenize(b, index);
    
    if (tokens[0].compare(endloop)) {
      std::cout << "Error: missing 'endloop'" << std::endl;
      return model;
    }
    
    index = newline_index(b, index+1);
    tokens = Tokenize(b, index);
    
    if (tokens[0].compare(endfacet)) {
      std::cout << "Error: missing 'endfacet'" << std::endl;
      return model;
    }
    
    Triangle::ShPtr t (new Triangle(verts[1], verts[2], verts[3], verts[0]) );
    model->add_triangle(t);
    triangles++;
    verts.clear();
    
    index = newline_index(b, index+1);
    tokens = Tokenize(b, index);
  }
  
  std::cout << "Read " << triangles << " triangles" << std::endl;
  
  return model;
}

// Returns the index of the first non-whitespace character after offset
const unsigned int StlDecoder::next_index(FileBlob& b, int offset) {
  
  while (is_whitespace(b[offset])) {
    offset++;
  }
  
  return offset++;
}

// Returns the index of the first character following a group of newline characters after the offset
const unsigned int StlDecoder::newline_index(FileBlob& b, int offset) {
  
  int ni = offset;
  
  while (!(b[ni] == '\n' || b[ni] == '\r')) {
    ni++;
  }
  while (b[ni] == '\n' || b[ni] == '\r') {
    ni++;
  }
  
  return ni++;
}

// Returns true if c is a whitespace character
const bool StlDecoder::is_whitespace(char c) {
  return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

//
std::vector<std::string> StlDecoder::Tokenize(FileBlob& b, int offset) {
  
  std::vector<std::string> tokens;
  int startOfString = next_index(b, offset);
  int endOfString = startOfString;
  int endOfLine = newline_index(b, offset);
  
  while (endOfString < endOfLine) {
    while (!is_whitespace(b[endOfString])) {
      endOfString++;
    }
    
    std::string str;
    str.assign(&b[startOfString], endOfString-startOfString);
    tokens.push_back(str);
    
    startOfString = next_index(b, endOfString);
    endOfString = startOfString;
  }
  
  return tokens;
}
