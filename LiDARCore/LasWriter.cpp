//
// Created by Han Hu on 3/2/17.
//

#include "LasWriter.h"

using namespace std;

LasWriter::LasWriter(const char *fileName, const LasDataset &lasDataset)
{
    createFileWithSize(fileName, lasDataset.getFileSize());

    writePublicHeaderBlock();
    writeVariableLengthRecords();
    writeExtendedVariableLengthRecords();

    // create write point function
    function<void(const LasPoint&)> fnWritePointDataRecordArr[11] = {
            bind(&LasWriter::writePointDataRecord0, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord1, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord2, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord3, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord4, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord5, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord6, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord7, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord8, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord9, this, placeholders::_1),
            bind(&LasWriter::writePointDataRecord10, this, placeholders::_1)};
    m_fnWritePointDataRecord = fnWritePointDataRecordArr[getPointDataRecordFormat()];

    m_byteStream.seek(getOffsetToPointData());
}

void LasWriter::writeExtendedVariableLengthRecords() const {
    if (!m_EVLRs.empty()) {
        m_byteStream.seek(m_publicHeaderBlock.startOfFirstExtendedVariableLengthRecord);
        for (size_t i = 0; i < m_EVLRs.size(); i++) {
            m_byteStream << m_EVLRs[i].header.reserved
                         << m_EVLRs[i].header.userID
                         << m_EVLRs[i].header.recordID
                         << m_EVLRs[i].header.recordLengthAfterHeader
                         << m_EVLRs[i].header.description;
            m_byteStream.writeBytes(m_EVLRs[i].record.get(), m_EVLRs[i].header.recordLengthAfterHeader);
        }
    }
}

void LasWriter::writePublicHeaderBlock() const {
    m_byteStream << m_publicHeaderBlock.fileSignature
                 << m_publicHeaderBlock.fileSourceID
                 << m_publicHeaderBlock.globalEncoding
                 << m_publicHeaderBlock.projectID_GUIDData1
                 << m_publicHeaderBlock.projectID_GUIDData2
                 << m_publicHeaderBlock.projectID_GUIDdata3
                 << m_publicHeaderBlock.projectID_GUIDData4
                 << m_publicHeaderBlock.versionMajor
                 << m_publicHeaderBlock.versionMinor
                 << m_publicHeaderBlock.systemIdentifier
                 << m_publicHeaderBlock.generatingSoftware
                 << m_publicHeaderBlock.fileCreationDayOfYear
                 << m_publicHeaderBlock.fileCreationYear
                 << m_publicHeaderBlock.headerSize
                 << m_publicHeaderBlock.offsetToPointData
                 << m_publicHeaderBlock.numberOfVariableLengthRecords
                 << m_publicHeaderBlock.pointDataRecordFormat
                 << m_publicHeaderBlock.pointDataRecordLength;

    if (m_publicHeaderBlock.versionMajor == 1 && m_publicHeaderBlock.versionMinor == 4) {
        m_byteStream << m_publicHeaderBlock.legacyNumberOfPointRecords
                     << m_publicHeaderBlock.legacyNumberOfPointsByReturn;
    } else {
        uint32_t numberOfPointRecords = m_publicHeaderBlock.numberOfPointRecords, numberOfPointsByReturn[5];
        for (size_t i = 0; i < 5; i++)
            numberOfPointsByReturn[i] = m_publicHeaderBlock.numberOfPointsByReturn[i];
        m_byteStream << numberOfPointRecords
                     << numberOfPointsByReturn;
    }

    m_byteStream << m_publicHeaderBlock.xScaleFactor
                 << m_publicHeaderBlock.yScaleFactor
                 << m_publicHeaderBlock.zScaleFactor
                 << m_publicHeaderBlock.xOffset
                 << m_publicHeaderBlock.yOffset
                 << m_publicHeaderBlock.zOffset
                 << m_publicHeaderBlock.maxX
                 << m_publicHeaderBlock.minX
                 << m_publicHeaderBlock.maxY
                 << m_publicHeaderBlock.minY
                 << m_publicHeaderBlock.maxZ
                 << m_publicHeaderBlock.minZ;
    if (m_publicHeaderBlock.versionMajor == 1 && m_publicHeaderBlock.versionMinor == 4) {
        m_byteStream << m_publicHeaderBlock.startOfWaveformDataPacketRecord
                     << m_publicHeaderBlock.startOfFirstExtendedVariableLengthRecord
                     << m_publicHeaderBlock.numberOfExtendedVariableLengthRecords
                     << m_publicHeaderBlock.numberOfPointRecords
                     << m_publicHeaderBlock.numberOfPointsByReturn;
    }
}

void LasWriter::writeVariableLengthRecords() const {
    // VARIABLE LENGTH RECORDS
    for (uint32_t i = 0; i < m_VLRs.size(); i++) {
        m_byteStream << m_VLRs[i].header.reserved
                     << m_VLRs[i].header.userID
                     << m_VLRs[i].header.recordID
                     << m_VLRs[i].header.recordLengthAfterHeader
                     << m_VLRs[i].header.description;
        uint16_t recordLengthAfterHeader = m_VLRs[i].header.recordLengthAfterHeader;
        m_byteStream.writeBytes(m_VLRs[i].record.get(), recordLengthAfterHeader);
    }
}

void LasWriter::writePointDataRecord0(const LasPoint &lasPoint) {
    m_byteStream.writeBytes(&lasPoint.basicPointData.record0, sizeof(POINT_DATA_RECORD_FORMAT_0));
}

void LasWriter::writePointDataRecord1(const LasPoint &lasPoint) {
    writePointDataRecord0(lasPoint);
    m_byteStream << lasPoint.GPSTime;
}

void LasWriter::writePointDataRecord2(const LasPoint &lasPoint) {
    writePointDataRecord0(lasPoint);
    m_byteStream << lasPoint.red
                 << lasPoint.green
                 << lasPoint.blue;
}

void LasWriter::writePointDataRecord3(const LasPoint &lasPoint) {
    writePointDataRecord1(lasPoint);
    m_byteStream << lasPoint.red
                 << lasPoint.green
                 << lasPoint.blue;
}

void LasWriter::writePointDataRecord4(const LasPoint &lasPoint) {
    writePointDataRecord1(lasPoint);
    writeWavePacket(lasPoint);
}

void LasWriter::writePointDataRecord5(const LasPoint &lasPoint) {
    writePointDataRecord3(lasPoint);
    writeWavePacket(lasPoint);
}

void LasWriter::writePointDataRecord6(const LasPoint &lasPoint) {
    m_byteStream.writeBytes(&lasPoint.basicPointData.record6, sizeof(POINT_DATA_RECORD_FORMAT_6));
}

void LasWriter::writePointDataRecord7(const LasPoint &lasPoint) {
    writePointDataRecord6(lasPoint);
    m_byteStream << lasPoint.red
                 << lasPoint.green
                 << lasPoint.blue;
}

void LasWriter::writePointDataRecord8(const LasPoint &lasPoint) {
    writePointDataRecord7(lasPoint);
    m_byteStream << lasPoint.NIR;
}

void LasWriter::writePointDataRecord9(const LasPoint &lasPoint) {
    writePointDataRecord6(lasPoint);
    writeWavePacket(lasPoint);
}

void LasWriter::writePointDataRecord10(const LasPoint &lasPoint) {
    writePointDataRecord7(lasPoint);
    writeWavePacket(lasPoint);
}

void LasWriter::writeWavePacket(const LasPoint &lasPoint) {
    m_byteStream << lasPoint.wavePacket.wavePacketDescriptionIndex
                 << lasPoint.wavePacket.byteOffsetToWaveformData
                 << lasPoint.wavePacket.waveformPacketSizeInBytes
                 << lasPoint.wavePacket.returnPointWaveformLocation
                 << lasPoint.wavePacket.X_t
                 << lasPoint.wavePacket.Y_t
                 << lasPoint.wavePacket.Z_t;
}
