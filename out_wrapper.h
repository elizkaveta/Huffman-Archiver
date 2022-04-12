#ifndef CPP_PILOT_HSE_OUT_WRAPPER_H
#define CPP_PILOT_HSE_OUT_WRAPPER_H

#include <fstream>
#include <vector>
#include <iostream>

class OutWrapper {
    using Code = int16_t;
public:
    OutWrapper(std::fstream& str);

    ~OutWrapper();

    std::fstream& stream;

    uint8_t GetChar() const;

    void AddCode(Code code, size_t size_of_code);

    void AddCode(std::vector<bool>& vec);

    void AddReverse(Code num, size_t size_of_num);

    void EndOutput();

protected:
    uint8_t* add_;

    static const size_t BITS_IN_BYTE = 8;
    static const size_t NUM_OF_MB = 1024 * 1024;

    void IfByteRead();

    char n_un_char_;
    size_t size_of_file_ = 0;
    size_t size_of_file_with_mb_ = 0;
    size_t size_of_free_place_ = BITS_IN_BYTE;
};

#endif  // CPP_PILOT_HSE_OUT_WRAPPER_H
