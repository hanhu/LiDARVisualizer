//
// Created by Han Hu on 3/2/17.
//

#ifndef LIDARVISUALIZER_LASWRITER_H
#define LIDARVISUALIZER_LASWRITER_H

#include "LasDataset.h"
#include "LasPoint.h"
#include "OutMemoryMappedFile.h"
#include "NonConstContainerBase.h"

class LasWriter : public virtual LasDataset, public  NonConstContainerBase<LasPoint>{
public:
    LasWriter(const char *fileName, const LasDataset &lasDataset);

    void create(const char *fileName, const PUBLIC_HEADER_BLOCK &publicHeaderBlock,
                    const std::vector<VARIABLE_LENGTH_RECORD> &VLRs, const std::vector<EXTENDED_VARIABLE_LENGTH_RECORD> &EVLRs);
private:
    void writeVariableLengthRecords() const;
    void writePublicHeaderBlock() const;
    void writeExtendedVariableLengthRecords() const;

    void writePointDataRecord0(const LasPoint &lasPoint);
    void writePointDataRecord1(const LasPoint &lasPoint);
    void writePointDataRecord2(const LasPoint &lasPoint);
    void writePointDataRecord3(const LasPoint &lasPoint);
    void writePointDataRecord4(const LasPoint &lasPoint);
    void writePointDataRecord5(const LasPoint &lasPoint);
    void writePointDataRecord6(const LasPoint &lasPoint);
    void writePointDataRecord7(const LasPoint &lasPoint);
    void writePointDataRecord8(const LasPoint &lasPoint);
    void writePointDataRecord9(const LasPoint &lasPoint);
    void writePointDataRecord10(const LasPoint &lasPoint);
    void writeWavePacket(const LasPoint &lasPoint);

    std::function<void(const LasPoint&)> m_fnWritePointDataRecord;
};


#endif //LIDARVISUALIZER_LASWRITER_H
