//
// Created by Han Hu on 2/23/17.
//

#ifndef LIDARVISUALIZER_LASREADER_H
#define LIDARVISUALIZER_LASREADER_H

#include <stdlib.h>
#include <functional>
#include "LasDataset.h"
#include "LasPoint.h"
#include "ConstContainerBase.h"
#include "Iterator.h"
#include "ConstIterator.h"

class LasReader : public LasDataset, public ConstContainerBase<LasPoint>{
public:
    LasReader(const char* fileName);

    void readKthElement(const uint64_t k) const override;
private:
    std::function<void(LasReader *)> m_fnRead;

    void readPointDataRecord0();
    void readPointDataRecord1();
    void readPointDataRecord2();
    void readPointDataRecord3();
    void readPointDataRecord4();
    void readPointDataRecord5();
    void readPointDataRecord6();
    void readPointDataRecord7();
    void readPointDataRecord8();
    void readPointDataRecord9();
    void readPointDataRecord10();

    void readWavePacket();
};


#endif //LIDARVISUALIZER_LASDATASET_H
