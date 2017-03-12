//
// Created by Han Hu on 3/1/17.
//

#include "KdTree.h"
#include "PrivatePointCloudDataset.h"
#include "LasWriter.h"
#include <fstream>
#include <iomanip>

using namespace std;

void KdTree::setDataSource(const shared_ptr<LasReader> pLasDataset) {
    m_pLasDataset = pLasDataset;
}

std::shared_ptr<LasReader> KdTree::getDataSource() const {
    return m_pLasDataset;
}

// Convert las dataset to private point cloud dataset
// And process iteratively on private point cloud datset to build KdTree
// Write las unorganized las dataset into organized dataset
void KdTree::build(const char *outLasFileName) {
    PrivatePointCloudDataset privatePointCloudDataset;
    string tempPrivatePointCloudDatasetFileName = tmpnam(nullptr);
    privatePointCloudDataset.create(tmpnam(nullptr), *getDataSource());
}