#include <iostream>

#include <LasWriter.h>
#include "LasReader.h"
#include "KdTree.h"
#include "LasReader.h"

using namespace std;

int main(int argc, char** argv) {
    const char* fileName = "NS_001_SW_PtCl.las";
    const char* outputFileName = "write_test.las";

    LasReader lasReader;
    lasReader.open(fileName);

    LasWriter lasWriter;
    lasWriter.create(outputFileName, lasReader.getPublicHeaderBlock(), lasReader.getVariableLengthRecords(), lasReader.getExtendedVariableLengthRecords());

    for (LasReader::const_iterator it = lasReader.begin(); it < lasReader.end(); it++) {

    }

    return 0;
}