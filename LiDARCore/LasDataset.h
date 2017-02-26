//
// Created by Han Hu on 2/23/17.
//

#ifndef LIDARVISUALIZER_LASDATASET_H
#define LIDARVISUALIZER_LASDATASET_H

#include <string>
#include "ErrorCode.h"

class LasDataset {
public:
    LasDataset();
    LasDataset(const std::string fileName);

    virtual ~LasDataset();

    LAS_ERROR open();

protected:
    std::string m_fileName;

    // PUBLIC HEADER BLOCK
    struct PUBLIC_HEADER_BLOCK {
        int8_t           fileSignature[4];
        uint16_t fileSourceID;
        uint16_t globalEncoding;
        uint32_t  projectID_GUIDData1;
        uint16_t projectID_GUIDData2;
        uint16_t projectID_GUIDdata3;
        uint8_t  projectID_GUIDData4[8];
        uint8_t  versionMajor;
        uint8_t  versionMinor;
        int8_t           systemIdentifier[32];
        int8_t           generatingSoftware[32];
        uint16_t fileCreationDayOfYear;
        uint16_t fileCreationYear;
        uint16_t headerSize;
        uint32_t  offsetToPointData;
        uint32_t  numberOfVariableLengthRecords;
        uint8_t  pointDataRecordFormat;
        uint8_t  pointDataRecordLength;
        uint32_t  legacyNumberOfPointRecords;
        uint32_t  legacyNumberOfPointsByReturn[5];
        double         xScaleFactor;
        double         yScaleFactor;
        double         zScaleFactor;
        double         xOffset;
        double         yOffset;
        double         zOffset;
        double         maxX;
        double         minX;
        double         maxY;
        double         minY;
        double         maxZ;
        double         minZ;
        uint64_t startOfWaveformDataPacketRecord;
        uint64_t startOfFirstExtendedVariableLengthRecord;
        uint32_t  numberOfExtendedVariableLengthRecord;
        union NumberOfPointRecords {
            uint32_t numberOfPointRecords32bit;
            uint64_t numberOfPointRecords64bit;
        } numberOfPointRecords;
        union NumberOfPointsByReturn {
            uint32_t numberOfPointsByReturn32bit[5];
            uint64_t numberOfPointsByReturn64bit[15];
        } numberOfPointsByReturn;
    } m_publicHeaderBlock;
};


#endif //LIDARVISUALIZER_LASDATASET_H
