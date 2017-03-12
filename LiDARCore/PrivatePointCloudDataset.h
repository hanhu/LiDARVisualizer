//
// Created by Han Hu on 3/3/17.
//

#ifndef LIDARVISUALIZER_PRIVATEPCDATASET_H
#define LIDARVISUALIZER_PRIVATEPCDATASET_H

#include <cstdint>
#include <string>
#include <boost/iostreams/device/mapped_file.hpp>
#include "LasReader.h"

struct PointXYZI{
    double X;
    double Y;
    double Z;
    uint64_t index;
};

class PrivatePointCloudDataset {
public:
    void create(const char *fileName, LasReader &lasReader);

protected:
    std::string m_fileName;
    boost::iostreams::mapped_file m_file;
    ByteStream m_byteStream;
};


#endif //LIDARVISUALIZER_PRIVATEPCDATASET_H
