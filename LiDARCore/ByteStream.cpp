//
// Created by Han Hu on 2/26/17.
//

#include "ByteStream.h"
#include <stdio.h>

ByteStream::ByteStream() {
    m_streamHead = m_curPos = nullptr;
}

ByteStream::ByteStream(char *stream) : m_streamHead(stream), m_curPos(stream) {

}

ByteStream::~ByteStream() {

}

void ByteStream::setStream(char *stream) {
    m_streamHead = m_curPos = stream;
}

void ByteStream::readBytes(void *dst, size_t size) {
    memcpy(dst, m_curPos, size);
    m_curPos += size;
}

void ByteStream::writeBytes(const void *src, size_t size) {
    memcpy(m_curPos, src, size);
    m_curPos += size;
}

char * ByteStream::head() const {
    return m_streamHead;
}

void ByteStream::seek(long int offset, int origin)
{
    if (origin == SEEK_SET){
        m_curPos = m_streamHead + offset;
    } else if (origin == SEEK_CUR) {
        m_curPos += offset;
    }
}

char* ByteStream::get() const {
    return m_curPos;
}