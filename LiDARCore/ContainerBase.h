//
// Created by Han Hu on 3/21/17.
//

#ifndef LIDARVISUALIZER_CONTAINERBASE_H
#define LIDARVISUALIZER_CONTAINERBASE_H

#include <stdlib.h>

template <typename value_type>
class ContainerBase {
public:
    ContainerBase() = default;
    explicit ContainerBase(const uint64_t size);
    const uint64_t size() const;

    uint64_t m_size;

    uint64_t getDataIndex() const;
    void setDataIndex(uint64_t m_dataIndex) const;

    mutable value_type                  m_dataItem;
    mutable uint64_t                      m_dataIndex;
};

template <typename value_type>
ContainerBase<value_type>::ContainerBase(const uint64_t size)
        :m_size(size)
{

}

template <typename value_type>
uint64_t ContainerBase<value_type>::getDataIndex() const {
    return m_dataIndex;
}

template <typename value_type>
void ContainerBase<value_type>::setDataIndex(uint64_t m_dataIndex) const {
    m_dataIndex = m_dataIndex;
}


#endif //LIDARVISUALIZER_CONTAINERBASE_H
