#ifndef CPP_PILOT_HSE_IN_WRAPPER_H
#define CPP_PILOT_HSE_IN_WRAPPER_H

#include <fstream>
#include <vector>


class InWrapper {
public:
    using Code = int16_t;

    InWrapper(std::fstream& str);

    std::fstream& stream;

    Code GetNextBit();

    Code GetBits(size_t num_of_bits);

protected:
    static const size_t BITS_IN_BYTE = 8;
    static const size_t SIZE_OF_ONE_CHAR = 1;
    size_t free_place_ = 0;
    char symbol_to_read_;
    uint8_t unsigned_symbol_;
};

#endif
