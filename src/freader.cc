#include <fstream>
#include <string>

#include "freader.h"

FileReader::FileReader(const std::string path)
    : file_path_(path),
      file_stream_(path.c_str(), std::ios::binary)  {
}

FileReader::~FileReader() {
  file_stream_.close();
}

// The other way to get the file length is via stat, but it's not supported on
// all platforms.
const int FileReader::GetFileSize() {
  file_stream_.seekg(0, std::ios::end);
  int length = file_stream_.tellg();
  file_stream_.seekg(std::ios::beg);
  return length;
}

// Copy the contents of the file into the supplied buffer
void FileReader::Read(char * buf, int fsize) {
  file_stream_.seekg(std::ios::beg);
  file_stream_.read(buf, fsize);
  file_stream_.seekg(std::ios::beg);
}

