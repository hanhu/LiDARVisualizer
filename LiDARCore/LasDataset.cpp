//
// Created by Han Hu on 2/23/17.
//

#include "LasDataset.h"
#include <iostream>
#include <boost/iostreams/device/mapped_file.hpp>


LasDataset::LasDataset() {

}

LasDataset::~LasDataset() {

}

LasDataset::LasDataset(const std::string fileName) : m_fileName(fileName){

}

LAS_ERROR LasDataset::open() {
    if (m_fileName.empty())
        return LAS_EMPTY_FILENAME;

    boost::iostreams::mapped_file_source file;
    file.open(m_fileName);

    if (!file.is_open())
        return LAS_FAIL;



    file.close();

    return LAS_OK;
}

