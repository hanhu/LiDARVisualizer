//
// Created by Han Hu on 2/23/17.
//

#include <sstream>
#include "LasReader.h"
#include "LasReader.h"

using namespace std;

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
    function<void(LasReader*)> fnReadPointDataRecordArr[] = {
            bind(&LasReader::readPointDataRecord0, placeholders::_1),
            bind(&LasReader::readPointDataRecord1, placeholders::_1),
            bind(&LasReader::readPointDataRecord2, placeholders::_1),
            bind(&LasReader::readPointDataRecord3, placeholders::_1),
            bind(&LasReader::readPointDataRecord4, placeholders::_1),
            bind(&LasReader::readPointDataRecord5, placeholders::_1),
            bind(&LasReader::readPointDataRecord6, placeholders::_1),
            bind(&LasReader::readPointDataRecord7, placeholders::_1),
            bind(&LasReader::readPointDataRecord8, placeholders::_1),
            bind(&LasReader::readPointDataRecord9, placeholders::_1),
            bind(&LasReader::readPointDataRecord10, placeholders::_1)};
    m_fnReadPointDataRecord = fnReadPointDataRecordArr[getPointDataRecordFormat()];

    m_lasPoint.init(getPointDataRecordFormat(), LasQuantizer(getXScaleFactor(), getYScaleFactor(), getZScaleFactor(), getXOffset(), getYOffset(), getZOffset()));
    m_byteStream.seek(getOffsetToPointData());
    m_fnReadPointDataRecord(this);
    m_pointIndex = 0;
}

void LasReader::close() {
    m_file.close();
}

LasReader::iterator LasReader::begin() {
    return LasReaderIterator(*this, 0);
}

LasReader::const_iterator LasReader::begin() const {
    return LasReaderConstIterator(*this, 0);
}

LasReader::iterator LasReader::end() {
    return LasReaderIterator(*this, getNumberOfPointRecords());
}

LasReader::const_iterator LasReader::end() const {
    return LasReaderConstIterator(*this, getNumberOfPointRecords());
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
    m_byteStream.readBytes(&m_lasPoint.basicPointData, sizeof(POINT_DATA_RECORD_FORMAT_0));
}

void LasReader::readPointDataRecord1() {
    readPointDataRecord0();
    m_byteStream >> m_lasPoint.GPSTime;
}

void LasReader::readPointDataRecord2() {
    readPointDataRecord0();
    m_byteStream >> m_lasPoint.red
                 >> m_lasPoint.green
                 >> m_lasPoint.blue;
}

void LasReader::readPointDataRecord3() {
    readPointDataRecord1();
     m_byteStream>> m_lasPoint.red
                 >> m_lasPoint.green
                 >> m_lasPoint.blue;
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
    m_byteStream.readBytes(&m_lasPoint.basicPointData, sizeof(POINT_DATA_RECORD_FORMAT_6));
}

void LasReader::readPointDataRecord7() {
    readPointDataRecord6();
    m_byteStream >> m_lasPoint.red
                 >> m_lasPoint.green
                 >> m_lasPoint.blue;
}

void LasReader::readPointDataRecord8() {
    readPointDataRecord7();
    m_byteStream >> m_lasPoint.NIR;
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
    m_byteStream >> m_lasPoint.wavePacket.wavePacketDescriptionIndex
                 >> m_lasPoint.wavePacket.byteOffsetToWaveformData
                 >> m_lasPoint.wavePacket.waveformPacketSizeInBytes
                 >> m_lasPoint.wavePacket.returnPointWaveformLocation
                 >> m_lasPoint.wavePacket.X_t
                 >> m_lasPoint.wavePacket.Y_t
                 >> m_lasPoint.wavePacket.Z_t;
}

void LasReader::readKthPoint(const uint64_t index) const {
    if (m_pointIndex != index) {
        m_byteStream.seek(getOffsetToPointData() + index * getPointDataRecordLength());
        m_fnReadPointDataRecord(const_cast<LasReader*>(this));
        m_pointIndex = index;
    }
}

LasReaderIterator::LasReaderIterator(LasReader &lasReader, uint64_t pointIndex)
: m_lasReader(lasReader), m_pointIndex(pointIndex){

}

LasPoint & LasReaderIterator::operator*() const {
    m_lasReader.readKthPoint(m_pointIndex);
    return m_lasReader.m_lasPoint;
}

void LasReaderIterator::operator++() {
    ++m_pointIndex;
}

void LasReaderIterator::operator++(int) {
    m_pointIndex++;
}

LasPoint * LasReaderIterator::operator->() const {
    m_lasReader.readKthPoint(m_pointIndex);
    return &m_lasReader.m_lasPoint;
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

const LasPoint & LasReaderConstIterator::operator*() const {
    m_lasReader.readKthPoint(m_pointIndex);
    return m_lasReader.m_lasPoint;
}

void LasReaderConstIterator::operator++() {
    ++m_pointIndex;
}

void LasReaderConstIterator::operator++(int) {
    m_pointIndex++;
}

const LasPoint * LasReaderConstIterator::operator->() const {
    m_lasReader.readKthPoint(m_pointIndex);
    return &m_lasReader.m_lasPoint;
}

bool LasReaderConstIterator::operator<(const LasReaderConstIterator &rhs) const {
    return m_pointIndex < rhs.m_pointIndex;
}