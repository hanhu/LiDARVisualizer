//
// Created by Han Hu on 3/3/17.
//

#include "PrivatePointCloudDataset.h"
#include "LasReader.h"
#include <iomanip>
#include <fstream>

using namespace std;

void PrivatePointCloudDataset::create(const char *fileName, LasReader &lasReader) {
    string strFileName = fileName;
    uint64_t numberOfBytes = sizeof(decltype(lasReader.getNumberOfPointRecords())) +
                             lasReader.getNumberOfPointRecords() * (3*sizeof(double) + sizeof(uint64_t));
    ofstream ofs(fileName, ios_base::out | ios_base::binary);
    ofs.seekp(numberOfBytes - 1);
    ofs.write("", 1);
    ofs.close();

    try {
        m_file.open(strFileName, boost::iostreams::mapped_file::readwrite, numberOfBytes);
    }
    catch(exception& e)
    {
        cerr << e.what() << endl;
    }

    m_byteStream.setStream(m_file.data());
    m_byteStream << lasReader.getNumberOfPointRecords();


    m_file.close();
}