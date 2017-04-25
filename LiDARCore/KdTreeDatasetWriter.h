//
// Created by Han Hu on 3/3/17.
//
// Self-defined point cloud format
// Number of points
// (X, Y, Z, Index)

#ifndef LIDARVISUALIZER_PRIVATEPCDATASET_H
#define LIDARVISUALIZER_PRIVATEPCDATASET_H

#include "LasReader.h"
#include "Vector.h"
#include "InMemoryMappedFile.h"

struct KDTREE_POINT_NODE{
    uint64_t index;
    uint64_t leftChildIndex;
    uint64_t rightChildIndex;
};

class KdTreeDatasetWriter : public InMemoryMappedFile, public OutMemoryMappedFile{
public:
    KdTreeDatasetWriter(const char* fileName, const LasReader& lasReader);

private:

    uint64_t findMedian(const LasReader &lasReader, const Vector<uint64_t> &indexes, const int splitDimension,
                            Vector<uint64_t> &left, Vector<uint64_t> &right);
};


#endif //LIDARVISUALIZER_PRIVATEPCDATASET_H
