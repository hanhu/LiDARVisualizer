//
// Created by Han Hu on 3/13/17.
//

#ifndef LIDARVISUALIZER_ITERATOR_H
#define LIDARVISUALIZER_ITERATOR_H

#include <stdlib.h>
#include <memory>
#include "ConstContainerBase.h"
#include "ConstIterator.h"

template <typename value_type>
class NonConstContainerBase;

template <typename value_type>
class Iterator {
public:
    Iterator(NonConstContainerBase<value_type>* pDataset, uint64_t index);

    value_type operator*();
    value_type*operator->();
    void operator++();
    void operator--();
    void operator++(int n);
    void operator--(int n);
    bool operator<(const Iterator<value_type>& rhs) const;
    bool operator>(const Iterator<value_type>& rhs) const;
    bool operator!=(const Iterator<value_type>& rhs) const;

    NonConstContainerBase<value_type>* m_pReader;
    uint64_t                   m_index;
};

template <typename value_type>
Iterator<value_type>::Iterator(NonConstContainerBase<value_type>* pDataset, uint64_t index)
        :m_pReader(pDataset), m_index(index){

}


template <typename element_type>
void Iterator<element_type>::operator++() {
    ++m_index;
}

template <typename element_type>
void Iterator<element_type>::operator--() {
    --m_index;
}

template <typename element_type>
void Iterator<element_type>::operator++(int n) {
    m_index++;
}

template <typename element_type>
void Iterator<element_type>::operator--(int n) {
    m_index--;
}

template <typename element_type>
bool Iterator<element_type>::operator<(const Iterator<element_type> &rhs) const {
    return m_index < rhs.m_index;
}

template <typename element_type>
bool Iterator<element_type>::operator>(const Iterator<element_type> &rhs) const {
    return m_index > rhs.m_index;
}

template <typename element_type>
bool Iterator<element_type>::operator!=(const Iterator<element_type> &rhs) const {
    return m_index != rhs.m_index;
}

#endif //LIDARVISUALIZER_ITERATOR_H
