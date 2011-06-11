#include <iostream>
#include <string>

#include "fileblob.h"
#include "freader.h"

FileBlob::FileBlob(std::string path) 
      : file_path_ (path),
        file_size_ (0),
	    bytes_ (NULL) {
  Init();
}

FileBlob::~FileBlob() {
  delete[] bytes_;
}

int FileBlob::Size() const {
  return file_size_;
}

void FileBlob::Init() {
  FileReader fr(file_path_);
  file_size_ = fr.GetFileSize();
  bytes_ = new char[file_size_];
  fr.Read(bytes_, file_size_);
}
