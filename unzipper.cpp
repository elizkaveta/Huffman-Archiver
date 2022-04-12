#include "unzipper.h"

#include <iostream>
#include <string>
#include <vector>

Unzipper::Unzipper() : in_(file_) {

}

// Открывают файлы
void Unzipper::UnOpenOutFile(std::fstream& out, const std::string& file_name) {
    out.open(file_name, std::ios::out | std::ios::trunc | std::ios_base::binary);
    if (!out.is_open()) {
        throw std::runtime_error("error - " + file_name + " is not opened");
    }
}

void Unzipper::UnOpenInFile(std::fstream& in, const std::string& file_name) {
    in.open(file_name, std::ios::in | std::ios_base::binary);
    if (!in.is_open()) {
        throw std::runtime_error("error - " + file_name + " is not opened");
    }
}

// не могут быть константными, так как меняется in
int16_t Unzipper::RetrieveSymbolsCount() { // Считывает размер алфавита
    return in_.GetBits(NUM_BITS_IN_CODE);
}

int16_t Unzipper::RetrieveSymbol() { // Получить 9-битный символ
    return in_.GetBits(NUM_BITS_IN_CODE);
}

int16_t Unzipper::RetrieveNumOfCodes() { // Считывает количества канонических кодов
    return in_.GetBits(NUM_BITS_IN_CODE);
}

void Unzipper::RetrieveAlphabet() { // Считывает алфавит
    for (Symbol& el : alphabet_) {
        el = RetrieveSymbol();
    }
}

void Unzipper::RetrieveSizesOfCodes() { // Считывает информацию о размерах кодов
    size_t sum = 0;
    size_t size_of_code = 1;
    size_t ind_of_symbol = 0;
    while (sum < alphabet_.size()) {
        size_t num = RetrieveNumOfCodes();
        sum += num;
        for (size_t i = 0; i < num; ++i) {
            sizes_of_codes_[ind_of_symbol++] = size_of_code;
        }
        ++size_of_code;
    }
    if (sum != alphabet_.size()) {
        throw std::runtime_error("Damaged archive");
    }
}

// Зная предыдущий канон. код, вычисляет следующий
void Unzipper::GetNextCode(std::vector<bool>& code_to_change, size_t new_size) const {
    for (size_t i = code_to_change.size() - 1; i >= 0; --i) {
        code_to_change[i] = 1 - code_to_change[i];
        if (code_to_change[i]) {
            break;
        }
        if (i == 0) {
            code_to_change.resize(code_to_change.size() + 1);
            code_to_change.assign(code_to_change.size() + 1, false);
            code_to_change[0] = true;
            break;
        }
    }
    code_to_change.resize(new_size);
}

void Unzipper::RetrieveCanonicalCodes() { // Создает каждому символу канонический код
    std::vector<bool> canon_code(sizes_of_codes_[0]);
    for (size_t i = 0; i < sizes_of_codes_.size(); ++i) {
        size_t size_of_code = sizes_of_codes_[i];
        Symbol symbol = alphabet_[i];
        if (i) {
            GetNextCode(canon_code, size_of_code);
        }
        code_of_symbol_[canon_code] = symbol;
    }
}

int16_t Unzipper::RetrieveEncodeSymbol() { // Извлекает следующий символ
    std::vector<bool> code;
    while (!code_of_symbol_.count(code)) {
        code.push_back(in_.GetNextBit());
        if (code.size() > sizes_of_codes_.back()) {
            throw std::runtime_error("Damaged archive");
        }
    }
    return code_of_symbol_[code];
}

std::string Unzipper::RetrieveFileName() { // Считывает имя файла
    Symbol symbol = RetrieveEncodeSymbol();
    std::string file_name;
    while (symbol != FILENAME_END) {
        file_name.push_back(symbol);
        symbol = RetrieveEncodeSymbol();
    }
    return file_name;
}

// Разархивирует файл, используя инфу о кодировании
void Unzipper::RetrieveFile(const std::string& file_name) {
    UnOpenOutFile(out_, file_name);
    Symbol symbol = RetrieveEncodeSymbol();
    while (symbol < FILENAME_END) {
        char u_sym = symbol;
        out_.write(&u_sym, 1);
        ++num_of_bytes_;
        if (num_of_bytes_ == NUM_IN_MB) {
            ++mb_in_files_;
            std::cout << mb_in_files_ << " Mb unarchived" << std::endl;
            num_of_bytes_ = 0;
        }
        symbol = RetrieveEncodeSymbol();
    }
    if (symbol == FILENAME_END) {
        throw std::runtime_error("Damaged archive");
    }
    if (symbol == ARCHIVE_END) {
        end_ = 1;
    }
}

void Unzipper::UnzipFile() { // Извлекает информацию о кодировании и печатает файл
    alphabet_.clear();
    sizes_of_codes_.clear();
    code_of_symbol_.clear();
    size_t size_of_alphabet = RetrieveSymbolsCount();
    alphabet_.resize(size_of_alphabet);
    sizes_of_codes_.resize(alphabet_.size());
    RetrieveAlphabet();
    RetrieveSizesOfCodes();
    RetrieveCanonicalCodes();
    std::string file_name = RetrieveFileName();
    RetrieveFile(file_name);
    out_.close();
}

// Разархивирует колекцию файлов
void Unzipper::Unzip(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        throw std::runtime_error("Invalid arguments. Use -h to get help.");
    }
    UnOpenInFile(file_, args[0]);
    while (!end_) {
        UnzipFile();
    }
}
