// Copyright (C) 2009 by Jhair Tocancipa Triana

#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <iostream>
#include <fstream>

class FileIOHandle
{
public:
  FileIOHandle(std::string fileName,
               std::ios_base::openmode mode = std::ios_base::in) :
    fileName(fileName),
    mode(mode)
  {
    fileStream = new std::fstream(fileName.c_str(), mode);
  }

  ~FileIOHandle()
  {
    fileStream->close();

    delete fileStream;
  }

  template<typename T>
  void read(T & obj,
            bool (T::*readLineHook)(const std::string &line))
  {
    std::string line;

    int line_no = 0;
    while (std::getline(*fileStream, line))
    {
      // Call the registered hook
      bool result = (obj.*readLineHook)(line);
      if (result == false)
      {
        std::cout << "Ignoring line..." << line << std::endl;
      }
    }
  }

  std::fstream *getFileStream()
  {
    return fileStream;
  }
  
private:
  std::string fileName;
  std::ios_base::openmode mode;
  std::fstream *fileStream;
};

#endif // FILEIO_HPP
