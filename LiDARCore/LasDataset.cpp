//
// Created by Han Hu on 2/23/17.
//

#include "LasDataset.h"
#include <iostream>
#include <boost/iostreams/device/mapped_file.hpp>
#include <functional>

using namespace std;

LasDataset::LasDataset() {

}

LasDataset::~LasDataset() {

}

LasDataset::LasDataset(const std::string fileName) : m_fileName(fileName){

}

template<typename T>
void readItem(uint8_t*& data, string itemName, T& item) {
    item = *reinterpret_cast<T*>(data);
    data += sizeof(T);
    cout << itemName << ": " << static_cast<double>(item) << endl;
}

template<typename T, const size_t size>
void readArrayItem(uint8_t*& data, string arrayItemName, T (&arrayItem)[size]) {
    for (size_t i = 0; i < size; i++) {
        arrayItem[i] = *reinterpret_cast<T *>(data);
        data += sizeof(T);
    }
    cout << arrayItemName << ": " << arrayItem << endl;
}

LAS_ERROR LasDataset::open() {
    if (m_fileName.empty())
        return LAS_EMPTY_FILENAME;

    boost::iostreams::mapped_file_source file;
    try {
        file.open(m_fileName);
    }
    catch (...) {
        return LAS_INVALID_FILENAME;
    }

    uint8_t* data = (uint8_t*)file.data();

    // Public Header Block

    // File Signature, char[4]
    readArrayItem(data, "File Signature", m_publicHeaderBlock.fileSignature);

    // File Source ID
    readItem(data, "File Source ID", m_publicHeaderBlock.fileSourceID);

    // Global Encoding
    readItem(data, "Global Encoding", m_publicHeaderBlock.globalEncoding);

    // Project ID - GUID data
    readItem(data, "Project ID - GUID data 1", m_publicHeaderBlock.projectID_GUIDData1);
    readItem(data, "Project ID - GUID data 2", m_publicHeaderBlock.projectID_GUIDData2);
    readItem(data, "Project ID - GUID data 3", m_publicHeaderBlock.projectID_GUIDdata3);
    readArrayItem(data, "Project ID - GUID data 4", m_publicHeaderBlock.projectID_GUIDData4);

    readItem(data, "Version Major", m_publicHeaderBlock.versionMajor);
    readItem(data, "Version Minor", m_publicHeaderBlock.versionMinor);

    readArrayItem(data, "System Identifier", m_publicHeaderBlock.systemIdentifier);
    readArrayItem(data, "Generating Software", m_publicHeaderBlock.generatingSoftware);
    readItem(data, "File Creation Day of Year", m_publicHeaderBlock.fileCreationDayOfYear);
    readItem(data, "File Creation Year", m_publicHeaderBlock.fileCreationYear);
    readItem(data, "Header Size", m_publicHeaderBlock.headerSize);
    readItem(data, "Offset to point data", m_publicHeaderBlock.offsetToPointData);
    readItem(data, "Number of Variable Length Records", m_publicHeaderBlock.numberOfVariableLengthRecords);
    readItem(data, "Point Data Record Format", m_publicHeaderBlock.pointDataRecordFormat);
    readItem(data, "Point Data Record Length", m_publicHeaderBlock.pointDataRecordLength);
    if (m_publicHeaderBlock.versionMajor == 1 && m_publicHeaderBlock.versionMinor == 4) {
        readItem(data, "Legacy Number of point records", m_publicHeaderBlock.legacyNumberOfPointRecords);
        readArrayItem(data, "Legacy Number of points by return", m_publicHeaderBlock.legacyNumberOfPointsByReturn);
    } else {
        readItem(data, "Number of point records", m_publicHeaderBlock.numberOfPointRecords.numberOfPointRecords32bit);
        readArrayItem(data, "Number of points by return", m_publicHeaderBlock.numberOfPointsByReturn.numberOfPointsByReturn32bit);
    }
    readItem(data, "X scale factor", m_publicHeaderBlock.xScaleFactor);
    readItem(data, "Y scale factor", m_publicHeaderBlock.yScaleFactor);
    readItem(data, "Z scale factor", m_publicHeaderBlock.zScaleFactor);
    readItem(data, "X offset", m_publicHeaderBlock.xOffset);
    readItem(data, "Y offset", m_publicHeaderBlock.yOffset);
    readItem(data, "Z offset", m_publicHeaderBlock.zOffset);
    readItem(data, "Max X", m_publicHeaderBlock.maxX);
    readItem(data, "Min X", m_publicHeaderBlock.minX);
    readItem(data, "Max Y", m_publicHeaderBlock.maxY);
    readItem(data, "Min Y", m_publicHeaderBlock.minY);
    readItem(data, "Max Z", m_publicHeaderBlock.maxZ);
    readItem(data, "Min Z", m_publicHeaderBlock.minZ);
    if (m_publicHeaderBlock.versionMajor == 1 && m_publicHeaderBlock.versionMinor == 4) {
        readItem(data, "Start of Waveform Data Packet Record", m_publicHeaderBlock.startOfWaveformDataPacketRecord);
        readItem(data, "Start of first Extended Variable Length Record", m_publicHeaderBlock.startOfFirstExtendedVariableLengthRecord);
        readItem(data, "Number of Extended Variable Length Record", m_publicHeaderBlock.numberOfExtendedVariableLengthRecord);
        readItem(data, "Number of point records", m_publicHeaderBlock.numberOfPointRecords.numberOfPointRecords64bit);
        readArrayItem(data, "Number of points by return", m_publicHeaderBlock.numberOfPointsByReturn.numberOfPointsByReturn64bit);
    }

    file.close();
    return LAS_OK;
}

