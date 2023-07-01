#ifndef PGM16_H
#define PGM16_H
#include "mat.h"
#include <string>
#include <fstream>
#include <iostream>

#define fendl std::endl
#define fout std::cout
#define flog std::clog
#define ferr std::cerr

extern bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue);
#endif