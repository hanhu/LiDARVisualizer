//
// Created by Han Hu on 3/13/17.
//

#ifndef LIDARVISUALIZER_CONSTITERATOR_H
#define LIDARVISUALIZER_CONSTITERATOR_H

#include "ConstContainerBase.h"
#include "Iterator.h"

template <typename element_type>
class ConstIterator {
public:
    ConstIterator(const ConstContainerBase<element_type>* pReader, const uint64_t index);

    const element_type&operator*() const;
    const element_type*operator->() const;
    void operator++() const ;
    void operator++(int n) const ;
    void operator--() const ;
    void operator--(int n) const ;
    bool operator<(const ConstIterator<element_type>& rhs) const;
    bool operator>(const ConstIterator<element_type>& rhs) const;
    bool operator!=(const ConstIterator<element_type>& rhs) const;

    const ConstContainerBase<element_type>* m_pReader;
    mutable uint64_t m_index;
};

template <typename element_type>
ConstIterator<element_type>::ConstIterator(const ConstContainerBase<element_type> *pReader, const uint64_t index)
        : m_pReader(pReader), m_index(index)
{

}

template <typename element_type>
const element_type& ConstIterator<element_type>::operator*() const {
    return m_pReader->at(m_index);
}

template <typename element_type>
const element_type* ConstIterator<element_type>::operator->() const {
    return &m_pReader->at(m_index);
}

template <typename element_type>
void ConstIterator<element_type>::operator++() const {
    ++m_index;
}

template <typename element_type>
void ConstIterator<element_type>::operator++(int n) const {
    m_index++;
}

template <typename element_type>
void ConstIterator<element_type>::operator--() const {
    --m_index;
}

template <typename element_type>
void ConstIterator<element_type>::operator--(int n) const {
    m_index--;
}

template <typename element_type>
bool ConstIterator<element_type>::operator<(const ConstIterator<element_type> &rhs) const {
    return m_index < rhs.m_index;
}

template <typename element_type>
bool ConstIterator<element_type>::operator>(const ConstIterator<element_type> &rhs) const {
    return m_index > rhs.m_index;
}

template <typename element_type>
bool ConstIterator<element_type>::operator!=(const ConstIterator<element_type> &rhs) const {
    return m_index != rhs.m_index;
}

#endif //LIDARVISUALIZER_CONSTITERATOR_H
