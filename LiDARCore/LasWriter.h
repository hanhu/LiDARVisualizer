//
// Created by Han Hu on 3/2/17.
//

#ifndef LIDARVISUALIZER_LASWRITER_H
#define LIDARVISUALIZER_LASWRITER_H

#include "LasDataset.h"

class LasWriter : public LasDataset{
public:
    LasWriter();
    virtual ~LasWriter();

    void create(const char *fileName, const PUBLIC_HEADER_BLOCK &publicHeaderBlock,
                    const std::vector<VARIABLE_LENGTH_RECORD> &VLRs, const std::vector<EXTENDED_VARIABLE_LENGTH_RECORD> &EVLRs);
    void writePointDataRecord(const POINT_DATA_RECORD& pdr);
    void close();
protected:

private:
    void writeVariableLengthRecords(const std::vector<VARIABLE_LENGTH_RECORD> &variableLengthRecords);
    void writePublicHeaderBlock(const PUBLIC_HEADER_BLOCK &publicHeaderBlock);

    void writePointDataRecord0(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord1(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord2(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord3(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord4(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord5(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord6(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord7(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord8(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord9(const POINT_DATA_RECORD& pdr);
    void writePointDataRecord10(const POINT_DATA_RECORD& pdr);
    void writeWavePacket(const POINT_DATA_RECORD& pdr);

    std::function<void(const POINT_DATA_RECORD&)> m_fnWritePointDataRecord;
};


#endif //LIDARVISUALIZER_LASWRITER_H
