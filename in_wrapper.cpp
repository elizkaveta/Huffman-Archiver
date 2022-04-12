#include "in_wrapper.h"

#include <fstream>


InWrapper::InWrapper(std::fstream& str) : stream(str) {

}

// Считывает следующий бит
InWrapper::Code InWrapper::GetNextBit() {
    if (!free_place_) {
        stream.read(&symbol_to_read_, SIZE_OF_ONE_CHAR);
        if (stream.gcount() != 1) {
            throw std::runtime_error("error - archive is damaged");
        }
        unsigned_symbol_ = static_cast<uint8_t>(symbol_to_read_);
        free_place_ = BITS_IN_BYTE;
    }
    Code ans = (unsigned_symbol_ >> (free_place_ - 1)) & 1;
    --free_place_;
    return ans;
}

// Считывает несколько следующих битов
InWrapper::Code InWrapper::GetBits(size_t num_of_bits) {
    if (num_of_bits == 0) {
        return 0;
    } else if (num_of_bits == 1) {
        return GetNextBit();
    } else {
        Code ans = GetNextBit();
        ans += (GetBits(num_of_bits - 1) << 1);
        return ans;
    }
}
