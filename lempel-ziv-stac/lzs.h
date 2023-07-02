#pragma once
#include <fstream>
#include <map>
extern void lzs_decompress(std::istream& is, std::ostream& os);
extern bool check_if_literal(uint8_t byte);
struct Bits
{
    protected:
        uint8_t byte;
        uint8_t validOffset;
    public:
        Bits(uint8_t byte);
        Bits();
        uint8_t peek_next_n_bits(uint8_t n);
        uint8_t get_next_n_bits(uint8_t n);
        void update_bits(Bits& next_bits);
        bool isClear();
        uint8_t clear();
        uint8_t size();
        bool isFull();
        void set(uint8_t byte);
};

enum states 
{
    newRead,
    literalInitialRead,
    nonLiteralInitialRead,
};