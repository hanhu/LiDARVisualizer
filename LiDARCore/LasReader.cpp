//
// Created by Han Hu on 2/23/17.
//

#include <sstream>
#include "LasReader.h"

using namespace std;

LasReader::LasReader(const char *fileName)
: ConstContainerBase<LasPoint>(fileName) {
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
                                            [](char *p) { delete[] p; });
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
                                                 [](char *p) { delete[] p; });
            m_byteStream.readBytes(m_EVLRs[i].record.get(), m_EVLRs[i].header.recordLengthAfterHeader);
        }
    }

    // Read the first point data record
    function<void(LasReader*)> fnReadPointDataRecordArr[] = {
            bind(&LasReader::readPointDataRecord0,   placeholders::_1),
            bind(&LasReader::readPointDataRecord1,   placeholders::_1),
            bind(&LasReader::readPointDataRecord2,   placeholders::_1),
            bind(&LasReader::readPointDataRecord3,   placeholders::_1),
            bind(&LasReader::readPointDataRecord4,   placeholders::_1),
            bind(&LasReader::readPointDataRecord5,   placeholders::_1),
            bind(&LasReader::readPointDataRecord6,   placeholders::_1),
            bind(&LasReader::readPointDataRecord7,   placeholders::_1),
            bind(&LasReader::readPointDataRecord8,   placeholders::_1),
            bind(&LasReader::readPointDataRecord9,   placeholders::_1),
            bind(&LasReader::readPointDataRecord10,  placeholders::_1)};

    m_fnRead = fnReadPointDataRecordArr[getPointDataRecordFormat()];

    m_byteStream.seek(getOffsetToPointData());
    m_fnRead(this);
    setDataIndex(0);
}

void LasReader::readPointDataRecord0() {
    m_byteStream.readBytes(&m_dataItem.basicPointData, sizeof(POINT_DATA_RECORD_FORMAT_0));
}

void LasReader::readPointDataRecord1() {
    readPointDataRecord0();
    m_byteStream >> m_dataItem.GPSTime;
}

void LasReader::readPointDataRecord2() {
    readPointDataRecord0();
    m_byteStream >> m_dataItem.red
                 >> m_dataItem.green
                 >> m_dataItem.blue;
}

void LasReader::readPointDataRecord3() {
    readPointDataRecord1();
     m_byteStream>> m_dataItem.red
                 >> m_dataItem.green
                 >> m_dataItem.blue;
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
    m_byteStream.readBytes(&m_dataItem.basicPointData, sizeof(POINT_DATA_RECORD_FORMAT_6));
}

void LasReader::readPointDataRecord7() {
    readPointDataRecord6();
    m_byteStream >> m_dataItem.red
                 >> m_dataItem.green
                 >> m_dataItem.blue;
}

void LasReader::readPointDataRecord8() {
    readPointDataRecord7();
    m_byteStream >> m_dataItem.NIR;
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
    m_byteStream >> m_dataItem.wavePacket.wavePacketDescriptionIndex
                 >> m_dataItem.wavePacket.byteOffsetToWaveformData
                 >> m_dataItem.wavePacket.waveformPacketSizeInBytes
                 >> m_dataItem.wavePacket.returnPointWaveformLocation
                 >> m_dataItem.wavePacket.X_t
                 >> m_dataItem.wavePacket.Y_t
                 >> m_dataItem.wavePacket.Z_t;
}

void LasReader::readKthElement(const uint64_t k) const {
    m_byteStream.seek(getOffsetToPointData() + k * getPointDataRecordLength());
    m_fnRead(const_cast<LasReader*>(this));
}
