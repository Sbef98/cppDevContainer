#include "lzs.h"
#include <iostream>
int main(void){
    std::ifstream is("prova.txt.lzs", std::ios::binary);
    lzs_decompress(is, std::cout);
}