//
// Created by Han Hu on 2/26/17.
//

#ifndef LIDARVISUALIZER_BYTESTREAM_H
#define LIDARVISUALIZER_BYTESTREAM_H

#include <iostream>
#include <stdio.h>
#include <sstream>

class ByteStream {
public:
    ByteStream();
    ByteStream(char* stream);

    virtual ~ByteStream();

    void setStream(char* stream);

    void readBytes(void* dst, size_t size);
    void writeBytes(const void* src, size_t size);

    char* head() const;
    char* get() const;
    void seek(long offset, int origin = SEEK_SET);

    template <typename T>
    ByteStream& operator >> (T& data)
    {
        const size_t numItemBytes = sizeof(T);
        uint8_t* itemBytes = (uint8_t *)(&data);
        for (size_t i = 0; i < numItemBytes; i++)
        {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            itemBytes[i] = m_curPos[i];
#else
            itemBytes[i] = data[numItemBytes - 1 - i];
#endif
        }
        m_curPos += numItemBytes;

        return *this;
    }

    template<typename T, const size_t size>
    ByteStream& operator >> (T (&arrayItem)[size]) {
        for (size_t i = 0; i < size; i++)
            (*this)>>(arrayItem[i]);
        return *this;
    }

    template <typename T>
    ByteStream& operator<<(const T& data)
    {
        const size_t numDataBytes = sizeof(T);
        uint8_t* itemBytes = (uint8_t*)(&data);
        for (size_t i = 0; i < numDataBytes; i++)
        {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            m_curPos[i] = itemBytes[i];
#else
            m_curPos[i] = itemBytes[numDataBytes - 1 - i];
#endif
        }

        m_curPos += numDataBytes;

        return *this;
    }

    template<typename T, const size_t size>
    ByteStream& operator << (const T (&arrayItem)[size]) {
        for (size_t i = 0; i < size; i++)
            (*this)<<(arrayItem[i]);
        return *this;
    }

protected:
    char* m_streamHead;
    char* m_curPos;
};


#endif //LIDARVISUALIZER_BYTESTREAM_H
