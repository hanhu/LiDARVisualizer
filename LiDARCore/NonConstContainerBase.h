//
// Created by Han Hu on 3/21/17.
//

#ifndef LIDARVISUALIZER_NONCONSTCONTAINERBASE_H
#define LIDARVISUALIZER_NONCONSTCONTAINERBASE_H

#include <fstream>
#include "OutMemoryMappedFile.h"
#include "ConstContainerBase.h"

template <typename element_type>
class Iterator;

template <typename value_type>
class NonConstContainerBase : public virtual OutMemoryMappedFile, public virtual ConstContainerBase<value_type>{
public:
    // Constructor
    NonConstContainerBase() = default;
    NonConstContainerBase(const uint64_t size);

    // functions that need to be overridden
    virtual void writeKthElement(const uint64_t k) const = 0;

    void reserve(uint64_t n);

    friend class Iterator<value_type>;
    using iterator = Iterator<value_type>;

    Iterator<value_type> begin();
    Iterator<value_type> end();
    Iterator<value_type> rbegin();
    Iterator<value_type> rend();

    value_type& at(const uint64_t n);
    value_type& operator[](const uint64_t n);

};

template <typename value_type >
NonConstContainerBase<value_type>::NonConstContainerBase(const uint64_t size)
: ContainerBase<value_type>(size)
{

}

template <typename value_type >
void NonConstContainerBase<value_type>::reserve(uint64_t n) {
    if (m_mmfile.is_open())
        m_mmfile.close();
    std::ofstream ofs(m_fileName, std::ios::binary | std::ios_base::out);
    ofs.seekp(n*sizeof(value_type) - 1);
    ofs.write("", 1);
    ofs.close();

    open(m_fileName.c_str());
}

template <typename value_type>
Iterator<value_type> NonConstContainerBase<value_type>::begin() {
    return Iterator<value_type>(this, 0);
}

template <typename value_type>
Iterator<value_type> NonConstContainerBase<value_type>::end() {
    return Iterator<value_type>(this, this->size());
}

template <typename value_type>
Iterator<value_type> NonConstContainerBase<value_type>::rbegin() {
    return end();
}

template <typename value_type>
Iterator<value_type> NonConstContainerBase<value_type>::rend() {
    return begin();
}


template <typename element_type>
element_type& NonConstContainerBase<element_type>::at(const uint64_t n) {
    if(this->getDataIndex() != n) {
        this->readKthElement(n);
        this->setDataIndex(n);
    }
    return this->m_dataItem;
}

template <typename element_type>
element_type& NonConstContainerBase<element_type>::operator[](const uint64_t n) {
    return at(n);
}

#endif //LIDARVISUALIZER_NONCONSTCONTAINERBASE_H
