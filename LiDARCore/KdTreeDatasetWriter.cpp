//
// Created by Han Hu on 3/17/17.
//

#include "KdTreeDatasetWriter.h"
#include <queue>

template <const int splitDimension>
bool compareLasPoint(LasPoint left, LasPoint right) {
    switch (splitDimension) {
        case 0:
            return left.getXCoordinate() < right.getXCoordinate();
        case 1:
            return left.getYCoordinate() < right.getYCoordinate();
        case 2:
            return left.getZCoordinate() < right.getZCoordinate();
        default:
            assert(false);
            return false;
    }
}

KdTreeDatasetWriter::KdTreeDatasetWriter(const char *fileName, const LasReader& lasReader)
{
    uint64_t fileSize = lasReader.size() * sizeof(KDTREE_POINT_NODE);
    OutMemoryMappedFile::createFileWithSize(fileName, fileSize);

    // build the kdtree
    int splitDimension = 0;

    // Initialize index vector
    Vector<uint64_t > indexes(lasReader.size());
    for (uint64_t i = 0; i < indexes.size(); i++)
        indexes[i] = i;

    std::queue<Vector<uint64_t >> q;
    q.push(indexes);
    while (!q.empty()) {
        Vector<uint64_t > front = q.front();
        Vector<uint64_t > left(std::ceil((front.size() - 1)/2.0f)), right(std::floor((front.size() - 1)/2.0f));
        uint64_t medianIndex = findMedian(lasReader, front, splitDimension, left, right);

        splitDimension = (++splitDimension)%3;
    }
}

uint64_t KdTreeDatasetWriter::findMedian(const LasReader &lasReader, const Vector<uint64_t> &indexes, const int splitDimension,
                                         Vector<uint64_t> &left, Vector<uint64_t> &right) {
    struct COORDINATE_WITH_INDEX {
        double X;
        double Y;
        double Z;
        uint64_t index;
    };

    // 1. Convert to a vector of {X,Y,Z,index}
    Vector<COORDINATE_WITH_INDEX> allPoints(lasReader.getNumberOfPointRecords());
    for (uint64_t i = 0; i < allPoints.size(); i++) {
    }

    return 0;
}