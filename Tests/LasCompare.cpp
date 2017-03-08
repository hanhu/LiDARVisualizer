//
// Created by Han Hu on 3/7/17.
//

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
    FILE* fp1 = fopen(argv[1], "r"), *fp2 = fopen(argv[2], "r");
    while (!feof(fp1)) {
        int c1 = fgetc(fp1);
        int c2 = fgetc(fp2);

        if (c1 != c2) {
            cout << "file differs at " << ftell(fp1) << "," << ftell(fp2) << endl;
            cout << c1 << "," << c2 << endl;
        }
    }
}