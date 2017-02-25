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
        char           fileSignature[4];
        unsigned short fileSourceID;
        unsigned short globalEncoding;
        unsigned long  projectID_GUIDData1;
        unsigned short projectID_GUIDData2;
        unsigned short projectID_GUIDDdata3;
        unsigned char  projectID_GUIDData4[8];
        unsigned char  versionMajor;
        unsigned char  versionMinor;
        char           systemIdentifier[32];
        char           generatingSoftware[32];
        unsigned short fileCreationDayOfYear;
        unsigned short fileCreationYear;
        unsigned short headerSize;
        unsigned long  offsetToPointData;
        unsigned long  numberOfVariableLengthRecords;
        unsigned char  pointDataRecordFormat;
        unsigned char  pointDataRecordLength;
        unsigned long  legacyNumberOfPointRecords;
        unsigned long  legacyNumberOfPointsByReturn[5];
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
        unsigned long long startOfWaveformDataPacketRecord;
        unsigned long long startOfFirstExtendedVariableLengthRecord;
        unsigned long  numberOfExtendedVariableLengthRecord;
        unsigned long long numberOfPointRecords;
        unsigned long long numberOfPointsByReturn[15];
    } m_publicHeaderBlock;
};


#endif //LIDARVISUALIZER_LASDATASET_H
