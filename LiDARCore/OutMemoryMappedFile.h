//
// Created by Han Hu on 3/20/17.
//

#ifndef LIDARVISUALIZER_OUTMEMORYMAPPEDFILE_H
#define LIDARVISUALIZER_OUTMEMORYMAPPEDFILE_H

#include <boost/iostreams/device/mapped_file.hpp>
#include "MemoryMappedFile.h"

class OutMemoryMappedFile : public virtual MemoryMappedFile{
public:
    void createFileWithSize(const char *fileName, const uint64_t fileSize);
};

#endif //LIDARVISUALIZER_OUTMEMORYMAPPEDFILE_H
