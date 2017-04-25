//
// Created by Han Hu on 3/17/17.
//

#ifndef LIDARVISUALIZER_VECTOR_H
#define LIDARVISUALIZER_VECTOR_H

#include "ConstContainerBase.h"
#include "OutMemoryMappedFile.h"
#include "NonConstContainerBase.h"
#include <vector>

template <typename value_type >
class Vector : public virtual ConstContainerBase<value_type>, public virtual NonConstContainerBase<value_type> {
public:
    Vector(const uint64_t size);
    Vector(const char* fileName, const uint64_t size);
    Vector(const std::vector<value_type>& vector);

    void readKthElement(const uint64_t k) const override;

    void writeKthElement(const uint64_t k) const override;

private:
    value_type m_value;
};

template <typename value_type >
Vector<value_type>::Vector(const uint64_t size)
: NonConstContainerBase<value_type>(size)
{
    char* tempFileName = tmpnam(nullptr);
    this->createFileWithSize(tempFileName, size * sizeof(value_type));
}

template <typename value_type >
Vector<value_type>::Vector(const std::vector<value_type> &vector)
: NonConstContainerBase<value_type>(vector.size())
{
    OutMemoryMappedFile::createFileWithSize(tmpnam(nullptr), sizeof(value_type) * vector.size());
    for (size_t i = 0; i < vector.size(); i++)
        this->m_byteStream << vector[i];
}

template <typename value_type>
void Vector<value_type>::writeKthElement(const uint64_t k) const {
    this->m_byteStream.seek(k*sizeof(value_type));
    this->m_byteStream << this->m_dataItem;
}

template <typename value_type>
void Vector<value_type>::readKthElement(const uint64_t k) const {
    this->m_byteStream.seek(k*sizeof(value_type));
    this->m_byteStream >> this->m_dataItem;
}

#endif //LIDARVISUALIZER_VECTOR_H
