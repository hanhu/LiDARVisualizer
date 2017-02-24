//
// Created by Han Hu on 2/23/17.
//

#ifndef LIDARVISUALIZER_LASDATASET_H
#define LIDARVISUALIZER_LASDATASET_H

#include <string>

class LasDataset {
public:
    LasDataset();
    LasDataset(const std::string fileName);

    virtual ~LasDataset();

    void open();

protected:
    std::string m_fileName;
};


#endif //LIDARVISUALIZER_LASDATASET_H
