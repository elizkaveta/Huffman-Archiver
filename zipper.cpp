#include "zipper.h"

#include <iostream>

#include "trie.h"

const size_t Zipper::NUM_OF_SPECIAL_BITS = 9;
const int16_t Zipper::FILENAME_END = 256;
const int16_t Zipper::ONE_MORE_FILE = 257;
const int16_t Zipper::ARCHIVE_END = 258;

Zipper::Zipper()
: out_(out_file_) {

}

// Открывают файлы
void Zipper::OpenOutFile(std::fstream& out, const std::string& file_name) {
    out.open(file_name, std::ios::out | std::ios::trunc | std::ios_base::binary);
    if (!out.is_open()) {
        throw std::runtime_error("error - " + file_name + " is not opened");
    }
}

void Zipper::OpenInFile(std::fstream& in, const std::string& file_name) {
    in.open(file_name, std::ios::in | std::ios_base::binary);
    if (!in.is_open()) {
        throw std::runtime_error("error - " + file_name + " is not opened");
    }
}

void Zipper::WriteFileName(const std::string& file_name) { // Кодирует имя файла
    for (const auto& i : file_name) {
        out_.AddCode(all_.canonical_forms[i]);
    }
    std::vector<bool> can_code;
    can_code = all_.canonical_forms[FILENAME_END];
    out_.AddCode(can_code);
}

void Zipper::WriteAlphabetAndCodes() { // Кодирует информацию об алфавите и кодах
    out_.AddReverse(all_.canonical_forms.size(), NUM_OF_SPECIAL_BITS);
    for (const auto& i : all_.symbol_of_code) {
        out_.AddReverse(i.second, NUM_OF_SPECIAL_BITS);
    }
    for (size_t i = 1; i <= all_.max_code_size; ++i) {
        out_.AddReverse(all_.count_of_codes_with_that_size[i], NUM_OF_SPECIAL_BITS);
    }
}

void Zipper::WriteFileContents(std::fstream& this_file) { // Кодирует содержимое файла
    char symbol;
    std::vector<bool> can_code;
    this_file.read(&symbol, 1);
    while (this_file.gcount() == 1) {
        uint8_t sym = symbol;
        can_code = all_.canonical_forms[sym];
        out_.AddCode(can_code);
        this_file.read(&symbol, 1);
    }
}

void Zipper::WriteFile(const std::string& file_name, bool end) { // Отвечает за архивирование файла
    std::fstream this_file_stream;
    OpenInFile(this_file_stream, file_name);
    WriteAlphabetAndCodes();
    WriteFileName(file_name);
    WriteFileContents(this_file_stream);
    if (end) {
        out_.AddCode(all_.canonical_forms[ARCHIVE_END]);
    } else {
        out_.AddCode(all_.canonical_forms[ONE_MORE_FILE]);
    }
    this_file_stream.close();
    all_.Clear();
}

// Ищет частоту символов в имени файла
void Zipper::RetrieveFrequencyFromFileName(const std::string& file_name) {
    for (const char& i : file_name) {
        frequency_[i]++;
    }
}

// Ищет частоту символов в содержимом файла
void Zipper::RetrieveFrequencyFromFile(const std::string& file_name) {
    char symbol;
    RetrieveFrequencyFromFileName(file_name);
    file_.read(&symbol, 1);
    while (file_.gcount() == 1) {
        uint8_t u_sym = symbol;
        frequency_[u_sym]++;
        file_.read(&symbol, 1);
    }
    frequency_[FILENAME_END] = 1;
    frequency_[ONE_MORE_FILE] = 1;
    frequency_[ARCHIVE_END] = 1;
    file_.close();
}

// Отвечает за все, чтобы сжать файл
void Zipper::ZipFile(const std::string& file_name, bool end) {
    OpenInFile(file_, file_name);
    RetrieveFrequencyFromFile(file_name);
    Trie trie(frequency_);
    trie.ChangeCodesToCanonical();
    all_ = trie.GetCodeInfo();
    WriteFile(file_name, end);
}

// Сжимает коллекцию файлов в данный
void Zipper::Zip(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        throw std::runtime_error("Invalid arguments. Use -h to get help.");
    }
    OpenOutFile(out_.stream, args[0]);
    bool is_end = false;
    for (size_t i = 1; i < args.size(); i++) {
        if (i + 1 == args.size()) {
            is_end = true;
        }
        ZipFile(args[i], is_end);
    }
    out_.EndOutput();
    out_.stream.close();
}
