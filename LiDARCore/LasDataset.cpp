//
// Created by Han Hu on 2/23/17.
//

#include "LasDataset.h"

using namespace std;

string LasDataset::getFileName() const {
    return m_fileName;
}

const PUBLIC_HEADER_BLOCK & LasDataset::getPublicHeaderBlock() const {
    return m_publicHeaderBlock;
}

uint16_t LasDataset::getHeaderSize() const {
    return m_publicHeaderBlock.headerSize;
}

const std::vector<VARIABLE_LENGTH_RECORD> & LasDataset::getVariableLengthRecords() const {
    return m_VLRs;
}

const std::vector<EXTENDED_VARIABLE_LENGTH_RECORD> LasDataset::getExtendedVariableLengthRecords() const {
    return m_EVLRs;
}

uint64_t LasDataset::getNumberOfPointRecords() const {
    return m_publicHeaderBlock.numberOfPointRecords;
}

string LasDataset::getVersionAsString() const {
    return to_string(getVersionMajor()) + "." + to_string(getVersionMinor());
}

uint8_t LasDataset::getVersionMajor() const {
    return m_publicHeaderBlock.versionMajor;
}

uint8_t LasDataset::getVersionMinor() const {
    return m_publicHeaderBlock.versionMinor;
}

uint32_t LasDataset::getNumberOfVariableLengthRecords() const {
    return m_publicHeaderBlock.numberOfVariableLengthRecords;
}

uint32_t LasDataset::getNumberOfExtendedVariableLengthRecords() const {
    return m_publicHeaderBlock.numberOfExtendedVariableLengthRecords;
}

uint8_t LasDataset::getPointDataRecordFormat() const {
    return m_publicHeaderBlock.pointDataRecordFormat;
}

uint32_t LasDataset::getOffsetToPointData() const {
    return m_publicHeaderBlock.offsetToPointData;
}

uint8_t LasDataset::getPointDataRecordLength() const {
    return m_publicHeaderBlock.pointDataRecordLength;
}

double LasDataset::getXScaleFactor() const {
    return m_publicHeaderBlock.xScaleFactor;
}

double LasDataset::getYScaleFactor() const {
    return m_publicHeaderBlock.yScaleFactor;
}

double LasDataset::getZScaleFactor() const {
    return m_publicHeaderBlock.zScaleFactor;
}

double LasDataset::getXOffset() const {
    return m_publicHeaderBlock.xOffset;
}

double LasDataset::getYOffset() const {
    return m_publicHeaderBlock.yOffset;
}

double LasDataset::getZOffset() const {
    return m_publicHeaderBlock.zOffset;
}