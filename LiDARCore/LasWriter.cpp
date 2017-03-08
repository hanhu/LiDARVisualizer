//
// Created by Han Hu on 3/2/17.
//

#include "LasWriter.h"
#include <fstream>

using namespace std;

LasWriter::LasWriter() {

}

LasWriter::~LasWriter() {

}

void LasWriter::create(const char *fileName, const PUBLIC_HEADER_BLOCK &publicHeaderBlock,
                       const std::vector<VARIABLE_LENGTH_RECORD> &VLRs, const std::vector<EXTENDED_VARIABLE_LENGTH_RECORD> &EVLRs)
{
    // Calculate the file size
    uint64_t fileSize = publicHeaderBlock.offsetToPointData + publicHeaderBlock.numberOfPointRecords * publicHeaderBlock.pointDataRecordLength;
    if (!EVLRs.empty()) {
        for (size_t i = 0; i < EVLRs.size(); i++) {
            fileSize += sizeof(EXTENDED_VARIABLE_LENGTH_RECORD_HEADER) + EVLRs[i].header.recordLengthAfterHeader;
        }
    }

    ofstream ofs(fileName, ios_base::binary | ios_base::out);
    ofs.seekp(fileSize - 1);
    ofs.write("", 1);
    ofs.close();

    m_file.open(fileName);
    m_byteStream.setStream(m_file.data());

    // Write public header block
    writePublicHeaderBlock(publicHeaderBlock);
    m_publicHeaderBlock = publicHeaderBlock;

    writeVariableLengthRecords(VLRs);
    m_VLRs = VLRs;

    if (!EVLRs.empty()) {
        m_byteStream.seek(publicHeaderBlock.startOfFirstExtendedVariableLengthRecord);
        for (size_t i = 0; i < EVLRs.size(); i++) {
            m_byteStream << EVLRs[i].header.reserved
                         << EVLRs[i].header.userID
                         << EVLRs[i].header.recordID
                         << EVLRs[i].header.recordLengthAfterHeader
                         << EVLRs[i].header.description;
            m_byteStream.writeBytes(EVLRs[i].record.get(), EVLRs[i].header.recordLengthAfterHeader);
        }
    }
    m_EVLRs = EVLRs;

    // create write point function
    auto test = bind(&LasWriter::writePointDataRecord0, this);

    function<void(const POINT_DATA_RECORD&)> fnWritePointDataRecordArr[11] = {
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

    // seek to the point data position
    m_byteStream.seek(getOffsetToPointData());
}

void LasWriter::writePointDataRecord(const POINT_DATA_RECORD& pdr)
{
    m_fnWritePointDataRecord(pdr);
}

void LasWriter::close() {
    m_file.close();
}

void LasWriter::writePublicHeaderBlock(const PUBLIC_HEADER_BLOCK &publicHeaderBlock) {
    m_byteStream << publicHeaderBlock.fileSignature
                 << publicHeaderBlock.fileSourceID
                 << publicHeaderBlock.globalEncoding
                 << publicHeaderBlock.projectID_GUIDData1
                 << publicHeaderBlock.projectID_GUIDData2
                 << publicHeaderBlock.projectID_GUIDdata3
                 << publicHeaderBlock.projectID_GUIDData4
                 << publicHeaderBlock.versionMajor
                 << publicHeaderBlock.versionMinor
                 << publicHeaderBlock.systemIdentifier
                 << publicHeaderBlock.generatingSoftware
                 << publicHeaderBlock.fileCreationDayOfYear
                 << publicHeaderBlock.fileCreationYear
                 << publicHeaderBlock.headerSize
                 << publicHeaderBlock.offsetToPointData
                 << publicHeaderBlock.numberOfVariableLengthRecords
                 << publicHeaderBlock.pointDataRecordFormat
                 << publicHeaderBlock.pointDataRecordLength;

    if (publicHeaderBlock.versionMajor == 1 && publicHeaderBlock.versionMinor == 4) {
        m_byteStream << publicHeaderBlock.legacyNumberOfPointRecords
                     << publicHeaderBlock.legacyNumberOfPointsByReturn;
    } else {
        uint32_t numberOfPointRecords = publicHeaderBlock.numberOfPointRecords, numberOfPointsByReturn[5];
        for (size_t i = 0; i < 5; i++)
            numberOfPointsByReturn[i] = publicHeaderBlock.numberOfPointsByReturn[i];
        m_byteStream << numberOfPointRecords
                     << numberOfPointsByReturn;
    }

    m_byteStream << publicHeaderBlock.xScaleFactor
                 << publicHeaderBlock.yScaleFactor
                 << publicHeaderBlock.zScaleFactor
                 << publicHeaderBlock.xOffset
                 << publicHeaderBlock.yOffset
                 << publicHeaderBlock.zOffset
                 << publicHeaderBlock.maxX
                 << publicHeaderBlock.minX
                 << publicHeaderBlock.maxY
                 << publicHeaderBlock.minY
                 << publicHeaderBlock.maxZ
                 << publicHeaderBlock.minZ;
    if (publicHeaderBlock.versionMajor == 1 && publicHeaderBlock.versionMinor == 4) {
        m_byteStream << publicHeaderBlock.startOfWaveformDataPacketRecord
                     << publicHeaderBlock.startOfFirstExtendedVariableLengthRecord
                     << publicHeaderBlock.numberOfExtendedVariableLengthRecords
                     << publicHeaderBlock.numberOfPointRecords
                     << publicHeaderBlock.numberOfPointsByReturn;
    }
}

void LasWriter::writeVariableLengthRecords(const vector<VARIABLE_LENGTH_RECORD> &variableLengthRecords) {
    // VARIABLE LENGTH RECORDS
    for (uint32_t i = 0; i < variableLengthRecords.size(); i++) {
        m_byteStream << variableLengthRecords[i].header.reserved
                     << variableLengthRecords[i].header.userID
                     << variableLengthRecords[i].header.recordID
                     << variableLengthRecords[i].header.recordLengthAfterHeader
                     << variableLengthRecords[i].header.description;
        uint16_t recordLengthAfterHeader = variableLengthRecords[i].header.recordLengthAfterHeader;
        m_byteStream.writeBytes(variableLengthRecords[i].record.get(), recordLengthAfterHeader);
    }
}

void LasWriter::writePointDataRecord0(const POINT_DATA_RECORD& pdr) {
    m_byteStream << pdr.iX
                 << pdr.iY
                 << pdr.iZ
                 << pdr.intensity;

    char ch = pdr.returnNumber | (pdr.numberOfReturns << 3) | (pdr.scanDirectionFlag << 6) | (pdr.edgeOfFlightLine << 7);
    m_byteStream << ch
                 << pdr.classification;
    char scanAngleRank = pdr.scanAngle;
    m_byteStream << scanAngleRank
                 << pdr.userData
                 << pdr.pointSourceID;
}

void LasWriter::writePointDataRecord1(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord0(pdr);
    m_byteStream << pdr.GPSTime;
}

void LasWriter::writePointDataRecord2(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord0(pdr);
    m_byteStream << pdr.red
                 << pdr.green
                 << pdr.blue;
}

void LasWriter::writePointDataRecord3(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord1(pdr);
    m_byteStream << pdr.red
                 << pdr.green
                 << pdr.blue;
}

void LasWriter::writePointDataRecord4(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord1(pdr);
    writeWavePacket(pdr);
}

void LasWriter::writePointDataRecord5(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord3(pdr);
    writeWavePacket(pdr);
}

void LasWriter::writePointDataRecord6(const POINT_DATA_RECORD& pdr) {
    m_byteStream << pdr.iX
                 << pdr.iY
                 << pdr.iZ
                 << pdr.intensity;

    char ch = pdr.returnNumber | (pdr.numberOfReturns << 4);
    m_byteStream << ch;
    ch = pdr.classificationFlags | (pdr.scannerChannel << 4) | (pdr.scanDirectionFlag << 6) | (pdr.edgeOfFlightLine << 7);
    m_byteStream << ch
                 << pdr.classification
                 << pdr.userData
                 << pdr.scanAngle
                 << pdr.pointSourceID
                 << pdr.GPSTime;
}

void LasWriter::writePointDataRecord7(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord6(pdr);
    m_byteStream << pdr.red
                 << pdr.green
                 << pdr.blue;
}

void LasWriter::writePointDataRecord8(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord7(pdr);
    m_byteStream << pdr.NIR;
}

void LasWriter::writePointDataRecord9(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord6(pdr);
    writeWavePacket(pdr);
}

void LasWriter::writePointDataRecord10(const POINT_DATA_RECORD& pdr) {
    writePointDataRecord7(pdr);
    writeWavePacket(pdr);
}

void LasWriter::writeWavePacket(const POINT_DATA_RECORD &pdr) {
    m_byteStream << pdr.wavePacketDescriptionIndex
                 << pdr.byteOFfsetToWaveformData
                 << pdr.waveformPacketSizeInBytes
                 << pdr.returnPointWaveformLocation
                 << pdr.X_t
                 << pdr.Y_t
                 << pdr.Z_t;
}