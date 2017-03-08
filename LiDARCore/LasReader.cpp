//
// Created by Han Hu on 2/23/17.
//

#include <sstream>
#include "LasReader.h"
#include "LasReader.h"

using namespace std;

LasReader::LasReader() {
}

LasReader::~LasReader() {

}

void LasReader::open(const char *fileName) {
    m_file.open(fileName);
    m_byteStream.setStream(m_file.data());

    // Read public header block
    m_byteStream >> m_publicHeaderBlock.fileSignature
                 >> m_publicHeaderBlock.fileSourceID
                 >> m_publicHeaderBlock.globalEncoding
                 >> m_publicHeaderBlock.projectID_GUIDData1
                 >> m_publicHeaderBlock.projectID_GUIDData2
                 >> m_publicHeaderBlock.projectID_GUIDdata3
                 >> m_publicHeaderBlock.projectID_GUIDData4
                 >> m_publicHeaderBlock.versionMajor
                 >> m_publicHeaderBlock.versionMinor
                 >> m_publicHeaderBlock.systemIdentifier
                 >> m_publicHeaderBlock.generatingSoftware
                 >> m_publicHeaderBlock.fileCreationDayOfYear
                 >> m_publicHeaderBlock.fileCreationYear
                 >> m_publicHeaderBlock.headerSize
                 >> m_publicHeaderBlock.offsetToPointData
                 >> m_publicHeaderBlock.numberOfVariableLengthRecords
                 >> m_publicHeaderBlock.pointDataRecordFormat
                 >> m_publicHeaderBlock.pointDataRecordLength;

    if (getVersionAsString() == "1.4") {
        m_byteStream >> m_publicHeaderBlock.legacyNumberOfPointRecords
                     >> m_publicHeaderBlock.legacyNumberOfPointsByReturn;
    } else {
        uint32_t numberOfPointRecords, numberOfPointRecordsByReturn[5];
        m_byteStream >> numberOfPointRecords
                     >> numberOfPointRecordsByReturn;
        m_publicHeaderBlock.numberOfPointRecords = numberOfPointRecords;
        for (size_t i = 0; i < 5; i++)
            m_publicHeaderBlock.numberOfPointsByReturn[i] = numberOfPointRecordsByReturn[i];
    }

    m_byteStream >> m_publicHeaderBlock.xScaleFactor
                 >> m_publicHeaderBlock.yScaleFactor
                 >> m_publicHeaderBlock.zScaleFactor
                 >> m_publicHeaderBlock.xOffset
                 >> m_publicHeaderBlock.yOffset
                 >> m_publicHeaderBlock.zOffset
                 >> m_publicHeaderBlock.maxX
                 >> m_publicHeaderBlock.minX
                 >> m_publicHeaderBlock.maxY
                 >> m_publicHeaderBlock.minY
                 >> m_publicHeaderBlock.maxZ
                 >> m_publicHeaderBlock.minZ;

    if (getVersionAsString() == "1.4") {
        m_byteStream >> m_publicHeaderBlock.startOfWaveformDataPacketRecord
                     >> m_publicHeaderBlock.startOfFirstExtendedVariableLengthRecord
                     >> m_publicHeaderBlock.numberOfExtendedVariableLengthRecords
                     >> m_publicHeaderBlock.numberOfPointRecords
                     >> m_publicHeaderBlock.numberOfPointsByReturn;
    }

    // Variable length records
    m_VLRs.resize(getNumberOfVariableLengthRecords());
    for (size_t i = 0; i < getNumberOfVariableLengthRecords(); i++) {
        m_byteStream >> m_VLRs[i].header.reserved
                     >> m_VLRs[i].header.userID
                     >> m_VLRs[i].header.recordID
                     >> m_VLRs[i].header.recordLengthAfterHeader
                     >> m_VLRs[i].header.description;
        m_VLRs[i].record = shared_ptr<char>(new char[m_VLRs[i].header.recordLengthAfterHeader],
        [](char* p){delete[] p;});
        m_byteStream.readBytes(m_VLRs[i].record.get(), m_VLRs[i].header.recordLengthAfterHeader);
    }

    if (getVersionAsString() == "1.4") {
        m_byteStream.seek(m_publicHeaderBlock.startOfFirstExtendedVariableLengthRecord);
        m_EVLRs.resize(getNumberOfExtendedVariableLengthRecords());
        for (size_t i = 0; i < getNumberOfExtendedVariableLengthRecords(); i++) {
            m_byteStream >> m_EVLRs[i].header.reserved
                         >> m_EVLRs[i].header.userID
                         >> m_EVLRs[i].header.recordID
                         >> m_EVLRs[i].header.recordLengthAfterHeader
                         >> m_EVLRs[i].header.description;
            m_EVLRs[i].record = shared_ptr<char>(new char[m_EVLRs[i].header.recordLengthAfterHeader],
            [](char* p){delete[] p;});
            m_byteStream.readBytes(m_EVLRs[i].record.get(), m_EVLRs[i].header.recordLengthAfterHeader);
        }
    }

    // Read the first point data record
    function<void(void)> fnReadPointDataRecordArr[] = {
            bind(&LasReader::readPointDataRecord0, this),
            bind(&LasReader::readPointDataRecord1, this),
            bind(&LasReader::readPointDataRecord2, this),
            bind(&LasReader::readPointDataRecord3, this),
            bind(&LasReader::readPointDataRecord4, this),
            bind(&LasReader::readPointDataRecord5, this),
            bind(&LasReader::readPointDataRecord6, this),
            bind(&LasReader::readPointDataRecord7, this),
            bind(&LasReader::readPointDataRecord8, this),
            bind(&LasReader::readPointDataRecord9, this),
            bind(&LasReader::readPointDataRecord10, this)};
    m_fnReadPointDataRecord = fnReadPointDataRecordArr[getPointDataRecordFormat()];

    m_byteStream.seek(getOffsetToPointData());
    m_fnReadPointDataRecord();
    m_pointIndex = 0;
}

LasReader::iterator LasReader::begin() {
    return LasReaderIterator(*this, 0);
}

LasReader::const_iterator LasReader::begin() const {
    return LasReaderConstIterator(*this, 0);
}

LasReader::iterator LasReader::end() {
    return LasReaderIterator(*this, getNumberOfPointRecords() - 1);
}

LasReader::const_iterator LasReader::end() const {
    return LasReaderConstIterator(*this, getNumberOfPointRecords() - 1);
}

LasReader::iterator LasReader::rbegin() {
    return end();
}

LasReader::const_iterator LasReader::rbegin() const {
    return end();
}

LasReader::iterator LasReader::rend() {
    return begin();
}

LasReader::const_iterator LasReader::rend() const {
    return begin();
}

void LasReader::readPointDataRecord0() {
    m_byteStream >> m_pdr.iX
                 >> m_pdr.iY
                 >> m_pdr.iZ
                 >> m_pdr.intensity;
    scalePoint();
    char c;
    m_byteStream >> c;
    m_pdr.returnNumber = c & 0x7;
    m_pdr.numberOfReturns = (c >> 3) & 0x7;
    m_pdr.scanDirectionFlag = (c >> 6) & 0x1;
    m_pdr.edgeOfFlightLine = (c >> 7) & 0x1;

    char scanAngleRank;
    m_byteStream >> m_pdr.classification
                 >> scanAngleRank
                 >> m_pdr.userData
                 >> m_pdr.pointSourceID;
    m_pdr.scanAngle = scanAngleRank;
}

void LasReader::scalePoint() const {
    m_pdr.X = m_pdr.iX * getXScaleFactor() + getXOffset();
    m_pdr.Y = m_pdr.iY * getYScaleFactor() + getYOffset();
    m_pdr.Z = m_pdr.iZ * getZScaleFactor() + getZOffset();
}

void LasReader::readPointDataRecord1() {
    readPointDataRecord0();
    m_byteStream >> m_pdr.GPSTime;
}

void LasReader::readPointDataRecord2() {
    readPointDataRecord0();
    m_byteStream >> m_pdr.red
                 >> m_pdr.green
                 >> m_pdr.blue;
}

void LasReader::readPointDataRecord3() {
    readPointDataRecord1();
     m_byteStream>> m_pdr.red
                 >> m_pdr.green
                 >> m_pdr.blue;
}

void LasReader::readPointDataRecord4() {
    readPointDataRecord1();
    readWavePacket();
}

void LasReader::readPointDataRecord5() {
    readPointDataRecord3();
    readWavePacket();
}

void LasReader::readPointDataRecord6() {
    m_byteStream >> m_pdr.iX
                 >> m_pdr.iY
                 >> m_pdr.iZ
                 >> m_pdr.intensity;
    scalePoint();
    char ch;
    m_byteStream >> ch;
    m_pdr.returnNumber = ch & 0xE;
    m_pdr.returnNumber = (ch >> 4) & 0xE;

    m_byteStream >> ch;
    m_pdr.classificationFlags = ch & 0xE;
    m_pdr.scannerChannel = (ch >> 4) & 0x3;
    m_pdr.scanDirectionFlag = (ch >> 6) & 0x1;
    m_pdr.edgeOfFlightLine = (ch >> 7) & 0x1;

    m_byteStream >> m_pdr.classification
                 >> m_pdr.userData
                 >> m_pdr.scanAngle
                 >> m_pdr.pointSourceID
                 >> m_pdr.GPSTime;
}

void LasReader::readPointDataRecord7() {
    readPointDataRecord6();
    m_byteStream >> m_pdr.GPSTime
                 >> m_pdr.red
                 >> m_pdr.green
                 >> m_pdr.blue;
}

void LasReader::readPointDataRecord8() {
    readPointDataRecord7();
    m_byteStream >> m_pdr.NIR;
}

void LasReader::readPointDataRecord9() {
    readPointDataRecord6();
    readWavePacket();
}

void LasReader::readPointDataRecord10() {
    readPointDataRecord7();
    readWavePacket();
}

void LasReader::readWavePacket()  {
    m_byteStream >> m_pdr.wavePacketDescriptionIndex
                 >> m_pdr.byteOFfsetToWaveformData
                 >> m_pdr.waveformPacketSizeInBytes
                 >> m_pdr.returnPointWaveformLocation
                 >> m_pdr.X_t
                 >> m_pdr.Y_t
                 >> m_pdr.Z_t;
}

void LasReader::readKthPoint(const uint64_t index) const {
    if (m_pointIndex != index) {
        m_byteStream.seek(getOffsetToPointData() + index * getPointDataRecordLength());
        m_fnReadPointDataRecord();
        m_pointIndex = index;
    }
}

LasReaderIterator::LasReaderIterator(LasReader &lasReader, uint64_t pointIndex)
: m_lasReader(lasReader), m_pointIndex(pointIndex){

}

POINT_DATA_RECORD& LasReaderIterator::operator*() const {
    m_lasReader.readKthPoint(m_pointIndex);
    return m_lasReader.m_pdr;
}

void LasReaderIterator::operator++() {
    ++m_pointIndex;
}

void LasReaderIterator::operator++(int) {
    m_pointIndex++;
}

POINT_DATA_RECORD* LasReaderIterator::operator->() const {
    m_lasReader.readKthPoint(m_pointIndex);
    return &m_lasReader.m_pdr;
}

bool LasReaderIterator::operator<(const LasReaderIterator &rhs) const {
    return m_pointIndex < rhs.m_pointIndex;
}

LasReaderConstIterator::LasReaderConstIterator(const LasReader &lasReader, uint64_t pointIndex)
: m_lasReader(lasReader), m_pointIndex(pointIndex){

}

LasReaderConstIterator::LasReaderConstIterator(const LasReaderIterator &iterator)
: m_lasReader(iterator.m_lasReader), m_pointIndex(iterator.m_pointIndex){

}

const POINT_DATA_RECORD& LasReaderConstIterator::operator*() const {
    m_lasReader.readKthPoint(m_pointIndex);
    return m_lasReader.m_pdr;
}

void LasReaderConstIterator::operator++() {
    ++m_pointIndex;
}

void LasReaderConstIterator::operator++(int) {
    m_pointIndex++;
}

const POINT_DATA_RECORD* LasReaderConstIterator::operator->() const {
    m_lasReader.readKthPoint(m_pointIndex);
    return &m_lasReader.m_pdr;
}

bool LasReaderConstIterator::operator<(const LasReaderConstIterator &rhs) const {
    return m_pointIndex < rhs.m_pointIndex;
}