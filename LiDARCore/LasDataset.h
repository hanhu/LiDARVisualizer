//
// Created by Han Hu on 2/23/17.
//

#ifndef LIDARVISUALIZER_LASDATASET_H
#define LIDARVISUALIZER_LASDATASET_H

#include <string>
#include <memory>
#include <vector>
#include <boost/iostreams/device/mapped_file.hpp>
#include "ByteStream.h"

// public header block
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
    uint16_t  pointDataRecordLength;
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
    uint32_t  numberOfExtendedVariableLengthRecords;
    uint64_t numberOfPointRecords;
    uint64_t numberOfPointsByReturn[15];
};

// variable length record header
struct VARIABLE_LENGTH_RECORD_HEADER {
    uint16_t reserved;
    char     userID[16];
    uint16_t recordID;
    uint16_t recordLengthAfterHeader;
    char     description[32];
};

// variable length record
struct VARIABLE_LENGTH_RECORD {
    VARIABLE_LENGTH_RECORD_HEADER header;
    std::shared_ptr<char> record;
};

// extended variable length record header
struct EXTENDED_VARIABLE_LENGTH_RECORD_HEADER {
    uint16_t    reserved;
    char        userID[16];
    uint16_t    recordID;
    uint64_t    recordLengthAfterHeader;
    char        description[32];
};

// extended variable length record
struct EXTENDED_VARIABLE_LENGTH_RECORD {
    EXTENDED_VARIABLE_LENGTH_RECORD_HEADER header;
    std::shared_ptr<char>                  record;
};

// las dataset
class LasDataset {
public:
    std::string getFileName() const;

    const PUBLIC_HEADER_BLOCK& getPublicHeaderBlock() const;
    const std::vector<VARIABLE_LENGTH_RECORD>& getVariableLengthRecords() const;
    const std::vector<EXTENDED_VARIABLE_LENGTH_RECORD> getExtendedVariableLengthRecords() const;

    uint16_t    getHeaderSize() const;
    uint64_t    getNumberOfPointRecords() const;
    std::string getVersionAsString() const;
    uint8_t     getVersionMajor() const;
    uint8_t     getVersionMinor() const;
    uint32_t    getNumberOfVariableLengthRecords() const;
    uint32_t    getNumberOfExtendedVariableLengthRecords() const;
    uint8_t     getPointDataRecordFormat() const;
    uint32_t    getOffsetToPointData() const;
    uint8_t     getPointDataRecordLength() const;

    // Scale factor and offset
    double getXScaleFactor() const;
    double getYScaleFactor() const;
    double getZScaleFactor() const;
    double getXOffset()      const;
    double getYOffset()      const;
    double getZOffset()      const;
protected:
    PUBLIC_HEADER_BLOCK m_publicHeaderBlock;
    std::vector<VARIABLE_LENGTH_RECORD> m_VLRs;
    std::vector<EXTENDED_VARIABLE_LENGTH_RECORD> m_EVLRs;

    std::string                   m_fileName;
    boost::iostreams::mapped_file m_file;
    mutable ByteStream                    m_byteStream;
};

#endif //LIDARVISUALIZER_LASDATASET_H
