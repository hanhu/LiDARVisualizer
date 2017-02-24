#include <iostream>
#include "LiDARCore/LasDataset.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage: visualizer filename" << endl;
        return -1;
    }

    LasDataset lasDS(argv[1]);

    // Open the dataset
    lasDS.open();

    return 0;
}