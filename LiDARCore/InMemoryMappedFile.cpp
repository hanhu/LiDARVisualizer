//
// Created by Han Hu on 3/20/17.
//

#include "InMemoryMappedFile.h"

InMemoryMappedFile::InMemoryMappedFile(const char *fileName)
: MemoryMappedFile(fileName)
{
    open(fileName);
}