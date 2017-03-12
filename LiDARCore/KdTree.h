//
// Created by Han Hu on 3/1/17.
//

#ifndef LIDARVISUALIZER_KDTREE_H
#define LIDARVISUALIZER_KDTREE_H

#include "LasReader.h"

class KdTree {
public:
    void setDataSource(const std::shared_ptr<LasReader> pLasDataset);
    std::shared_ptr<LasReader> getDataSource() const;
    void build(const char *outLasFileName);

protected:
    std::shared_ptr<LasReader> m_pLasDataset;
};


#endif //LIDARVISUALIZER_KDTREE_H
