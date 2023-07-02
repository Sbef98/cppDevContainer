#include "lzs.h"
Bits::Bits(){
    this->validOffset = 0;
}
Bits::Bits(uint8_t byte)
{
    this->byte = byte;
    this->validOffset = 8;
}

uint8_t Bits::peek_next_n_bits(uint8_t n)
{
    return this->byte >> (8 - n);
}

uint8_t Bits::get_next_n_bits(uint8_t n)
{
    uint8_t returnValue = this->peek_next_n_bits(n);
    this->byte = this->byte << n;
    this->validOffset = this->validOffset - n;
    return returnValue;
}

uint8_t Bits::size() {
    return this->validOffset;
}

void Bits::update_bits(Bits& next_bits)
{
    uint8_t n = 8 - this->validOffset;
    if(n > next_bits.size()){
        n = next_bits.size();
    }
    uint8_t tmp = next_bits.get_next_n_bits(n);
    tmp = tmp >> this->validOffset;
    this->byte = this->byte | tmp;
    this->validOffset = this->validOffset + n;
}

bool Bits::isClear(){
    if(this->validOffset == 0){
        return true;
    } else {
        return false;
    }
}

uint8_t Bits::clear(){
    const uint8_t returnValue = this->byte >> (8 - this->validOffset);
    this->validOffset = 0;
    return returnValue;
}

bool Bits::isFull(){
    if(this->validOffset == 8){
        return true;
    } else {
        return false;
    }
}

void Bits::set(uint8_t byte){
    this->byte = byte;
    this->validOffset = 8;
}

bool check_if_literal(Bits& byte){
    const uint8_t initial_bit = byte.peek_next_n_bits(1);
    if(initial_bit == 0){
        byte.get_next_n_bits(1);
        return true;
    } else {
        return false;
    }
}

void lzs_decompress(std::istream& is, std::ostream& os){
    states state = newRead;
    int next_byte = is.get();
    if(next_byte == EOF)
        return;
    Bits current_bits(static_cast<uint8_t>(next_byte));
    Bits next_bits{};

    do
    {
        if(next_bits.isClear()){
            next_byte = is.get();
            if(next_byte == EOF){
                exit(0);
            }
            next_bits.set(static_cast<uint8_t>(next_byte));
        }
        
        if(state == newRead && !current_bits.isClear() && check_if_literal(current_bits)){
            state = literalInitialRead;
            current_bits.update_bits(next_bits);
            continue;
        }
        if(state == literalInitialRead && current_bits.isFull()){
            os << current_bits.clear();
            current_bits.update_bits(next_bits);
            state = newRead;
            continue;
        }
        if(state == literalInitialRead && (!current_bits.isFull() || current_bits.isClear())){
            current_bits.update_bits(next_bits);
            continue;
        }

    } while (is.peek() != EOF);
    
}