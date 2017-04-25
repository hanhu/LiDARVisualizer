//
// Created by Han Hu on 3/13/17.
//

#ifndef LIDARVISUALIZER_MEMORYMAPPEDDATASET_H
#define LIDARVISUALIZER_MEMORYMAPPEDDATASET_H

#include <boost/iostreams/device/mapped_file.hpp>
#include "ByteStream.h"

class MemoryMappedFile{
public:
    MemoryMappedFile();                    // Default constructor is not implicitly declared as there is a user-declared constructor
    MemoryMappedFile(const char* fileName);
    virtual ~MemoryMappedFile();

    std::string getFileName();

    void open(const char *fileName);

protected:
    boost::iostreams::mapped_file m_mmfile;
    std::string                   m_fileName;
    mutable ByteStream                    m_byteStream;

};


#endif //LIDARVISUALIZER_MEMORYMAPPEDDATASET_H
