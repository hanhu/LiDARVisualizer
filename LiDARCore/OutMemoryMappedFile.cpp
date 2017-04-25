//
// Created by Han Hu on 3/20/17.
//

#include "MemoryMappedFile.h"
#include "OutMemoryMappedFile.h"
#include <fstream>

void OutMemoryMappedFile::createFileWithSize(const char *fileName, const uint64_t fileSize) {
    std::ofstream ofs(fileName, std::ios_base::binary | std::ios_base::out);
    ofs.seekp(fileSize - 1);
    ofs.write("", 1);
    ofs.close();

    open(fileName);
}