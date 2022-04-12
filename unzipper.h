#ifndef CPP_PILOT_HSE_UNZIPPER_H
#define CPP_PILOT_HSE_UNZIPPER_H

#include <map>
#include <string>
#include <unordered_map>

#include "in_wrapper.h"
#include "compare_codes.h"


class Unzipper {
public:
    using Symbol = int16_t;

    Unzipper();

    void Unzip(const std::vector<std::string>& args);

protected:
    static const Symbol FILENAME_END = 256;
    static const Symbol ONE_MORE_FILE = 257;
    static const Symbol ARCHIVE_END = 258;
    static const size_t NUM_BITS_IN_CODE = 9;
    static const size_t NUM_IN_MB = 1024 * 1024;

    std::vector<Symbol> alphabet_;
    std::vector<size_t> sizes_of_codes_;
    std::unordered_map<std::vector<bool>, Symbol> code_of_symbol_;
    std::fstream out_;
    std::fstream file_;
    bool end_ = 0;
    InWrapper in_;
    size_t num_of_bytes_ = 0;
    size_t mb_in_files_ = 0;

    Symbol RetrieveSymbolsCount(); // Считает размер алфавита

    Symbol RetrieveSymbol(); //

    void RetrieveAlphabet(); // Извлекает алфавит

    Symbol RetrieveNumOfCodes(); //

    void RetrieveSizesOfCodes();

    void RetrieveCanonicalCodes();

    std::string RetrieveFileName();

    Symbol RetrieveEncodeSymbol();

    void UnzipFile();

    void RetrieveFile(const std::string& file_name); // Разархивирует конкретный файл

    void UnOpenOutFile(std::fstream& out, const std::string& file_name);

    void UnOpenInFile(std::fstream& in, const std::string& file_name);

    void GetNextCode(std::vector<bool>& code_to_change, size_t new_size) const;
};

#endif  // CPP_PILOT_HSE_UNZIPPER_H
