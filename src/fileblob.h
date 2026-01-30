#ifndef TDC_FILEBLOB_H_
#define TDC_FILEBLOB_H_

#include <string>

#include "basetypes.h"

// A wrapper for the binary contents of a file. Makes the bytes of a file available like an array.
class FileBlob {

 public:
  // Build a new instance based on a supplied path
  explicit FileBlob(std::string path);
  ~FileBlob();
  const char& operator[](std::size_t position) { return bytes_[position]; }
  int Size() const;

 private:
  void Init();

  std::string file_path_;
  int file_size_;
  
  char * bytes_;

  DISALLOW_COPY_AND_ASSIGN(FileBlob);
};

#endif
