//
// Created by Han Hu on 3/20/17.
//

#ifndef LIDARVISUALIZER_INMEMORYMAPPEDFILE_H
#define LIDARVISUALIZER_INMEMORYMAPPEDFILE_H

#include "MemoryMappedFile.h"

class InMemoryMappedFile : public virtual MemoryMappedFile{
public:
    InMemoryMappedFile() = default;
    InMemoryMappedFile(const char* fileName);
};


#endif //LIDARVISUALIZER_INMEMORYMAPPEDFILE_H
