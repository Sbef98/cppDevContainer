#include "pgm16.h"
#include <iostream>
#include <string>

#define frog "frog_bin.pgm"
#define chest "CR-MONO1-10-chest.pgm"

int main(void){
    const std::string filename = frog;
    mat<uint16_t> mat;
    uint16_t maxValue;
    load(filename, mat, maxValue);
    return 0;
}