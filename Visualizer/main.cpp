#include <iostream>
#include <LasWriter.h>
#include "LasReader.h"
#include "KdTree.h"
#include "LasReader.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage: visualizer filename" << endl;
        return -1;
    }

    LasReader lasReader;

    // Open the dataset
    lasReader.open(argv[1]);

    // Print out basic las dataset information
    cout << "Las version: " << lasReader.getVersionAsString() << endl;
    cout << "Number of points: " << lasReader.getNumberOfPointRecords() << endl;
    cout << "Point data record format: " << to_string(lasReader.getPointDataRecordFormat()) << endl;

    LasWriter lasWriter;
    lasWriter.create("write.las", lasReader.getPublicHeaderBlock(), lasReader.getVariableLengthRecords(), lasReader.getExtendedVariableLengthRecords());

    for (LasReader::const_iterator it = lasReader.begin(); it < lasReader.end(); it++) {
        lasWriter.writePointDataRecord(*it);
    }

    lasWriter.close();

    return 0;
}