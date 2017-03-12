//
// Created by Han Hu on 3/2/17.
//

#ifndef LIDARVISUALIZER_LASWRITER_H
#define LIDARVISUALIZER_LASWRITER_H

#include "LasDataset.h"
#include "LasPoint.h"

class LasWriter : public LasDataset{
public:
    void create(const char *fileName, const PUBLIC_HEADER_BLOCK &publicHeaderBlock,
                    const std::vector<VARIABLE_LENGTH_RECORD> &VLRs, const std::vector<EXTENDED_VARIABLE_LENGTH_RECORD> &EVLRs);
    void writePointDataRecord(const LasPoint &lasPoint);
    void close();
protected:

private:
    void writeVariableLengthRecords(const std::vector<VARIABLE_LENGTH_RECORD> &variableLengthRecords);
    void writePublicHeaderBlock(const PUBLIC_HEADER_BLOCK &publicHeaderBlock);

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
