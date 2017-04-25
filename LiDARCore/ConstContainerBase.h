//
// Created by Han Hu on 3/14/17.
//

#ifndef LIDARVISUALIZER_CONSTCONTAINERBASE_H
#define LIDARVISUALIZER_CONSTCONTAINERBASE_H

#include "MemoryMappedFile.h"
#include "InMemoryMappedFile.h"
#include "ContainerBase.h"

template <typename element_type>
class ConstIterator;

template <typename value_type>
class ConstContainerBase : public virtual InMemoryMappedFile, public virtual ContainerBase<value_type>{
public:
    ConstContainerBase() = default;
    explicit ConstContainerBase(const char* fileName);

    // Pure virtual functions that need to be override
    virtual void readKthElement(const uint64_t k) const = 0;

    friend class ConstIterator<value_type>;
    using const_iterator = ConstIterator<value_type>;

    ConstIterator<value_type> cbegin() const;

    ConstIterator<value_type> cend() const;

    ConstIterator<value_type> crbegin() const;

    ConstIterator<value_type> crend() const;

    const value_type& at(const uint64_t n) const;
    const value_type&operator[](const uint64_t n) const;
};

template <typename element_type>
ConstContainerBase<element_type>::ConstContainerBase(const char *fileName)
        : MemoryMappedFile(fileName)
{

}

template <typename element_type>
ConstIterator<element_type> ConstContainerBase<element_type>::cbegin() const {
    return ConstIterator<element_type>(this, 0);
}

template <typename element_type>
ConstIterator<element_type> ConstContainerBase<element_type>::cend() const{
    return ConstIterator<element_type>(this, this->size());
}


template <typename element_type>
ConstIterator<element_type> ConstContainerBase<element_type>::crbegin() const {
    return cend();
}

template <typename element_type>
ConstIterator<element_type> ConstContainerBase<element_type>::crend() const {
    return cbegin();
}

#endif //LIDARVISUALIZER_MEMORYMAPPEDDATASETREADER_H
