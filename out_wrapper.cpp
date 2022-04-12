#include "out_wrapper.h"

#include <fstream>


OutWrapper::OutWrapper(std::fstream& str)
        : stream(str) {
    add_ = new uint8_t[1];
    add_[0] = 0;
}

// Проверяет, заполнен ли char add_ (чтобы записать его в файл)
void OutWrapper::IfByteRead() {
    if (size_of_free_place_ == 0) {
        n_un_char_ = add_[0];
        size_of_free_place_ = BITS_IN_BYTE;
        stream.write(&n_un_char_, 1);
        ++size_of_file_with_mb_;
        if (size_of_file_with_mb_ == NUM_OF_MB) {
            ++size_of_file_;
            std::cout << size_of_file_ << " Mb archived" << std::endl;
            size_of_file_with_mb_ = 0;
        }
        add_[0] = 0;
    }
}

// Добавляет код code
void OutWrapper::AddCode(Code code, size_t size_of_code) {
    if (!size_of_code) {
        return;
    }
    if (size_of_code <= size_of_free_place_) {
        Code part_of_code = code << (size_of_free_place_ - size_of_code);
        add_[0] |= part_of_code;
        size_of_free_place_ -= size_of_code;
        IfByteRead();
    } else {
        add_[0] += code >> (size_of_code - size_of_free_place_);
        code -= (code >> (size_of_code - size_of_free_place_))
                << (size_of_code - size_of_free_place_);
        size_t remainder = size_of_code - size_of_free_place_;
        size_of_free_place_ = 0;
        IfByteRead();
        AddCode(code, remainder);
    }
}

// Добавляет код в виде вектора
void OutWrapper::AddCode(std::vector<bool>& vec) {
    Code add_number;
    size_t ind = 0;
    while (ind < vec.size()) {
        add_number = 0;
        size_t size_of_code;
        for (size_of_code = 0; size_of_code < BITS_IN_BYTE && ind < vec.size(); ++ind, ++size_of_code) {
            add_number <<= 1;
            add_number += vec[ind];
        }
        AddCode(add_number, size_of_code);
    }
}

// Добавляет код в литл эндиан
void OutWrapper::AddReverse(Code num, size_t size_of_num) {
    Code add_number = 0;
    for (size_t i = 0; i < size_of_num; ++i) {
        add_number = (add_number << 1) + (num & 1);
        num >>= 1;
    }
    AddCode(add_number, size_of_num);
}

// Функция для вывода самого последнего байта
void OutWrapper::EndOutput() {
    if (size_of_free_place_ != BITS_IN_BYTE) {
        n_un_char_ = add_[0];
        stream.write(&n_un_char_, 1);
        add_[0] = 0;
        size_of_free_place_ = BITS_IN_BYTE;
    }
}

OutWrapper::~OutWrapper() {
    delete add_;
}

// Возвращает текущий байт
uint8_t OutWrapper::GetChar() const {
    return add_[0];
}
