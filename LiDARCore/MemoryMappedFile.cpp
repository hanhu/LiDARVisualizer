//
// Created by Han Hu on 3/13/17.
//

#include "MemoryMappedFile.h"
#include "OutMemoryMappedFile.h"
#include <fstream>

MemoryMappedFile::MemoryMappedFile()
: m_fileName("") {
    m_byteStream.setStream(nullptr);
}

MemoryMappedFile::MemoryMappedFile(const char* fileName)
: m_fileName(fileName)
{
}

MemoryMappedFile::~MemoryMappedFile() {
    if (m_mmfile.is_open())
        m_mmfile.close();
    m_byteStream.setStream(nullptr);
}

std::string MemoryMappedFile::getFileName() {
    return m_fileName;
}

void MemoryMappedFile::open(const char *fileName) {
    m_fileName = fileName;
    m_mmfile.open(fileName);
    m_byteStream.setStream(m_mmfile.data());
}

