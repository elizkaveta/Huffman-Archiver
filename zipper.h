#ifndef CPP_PILOT_HSE_ZIPPER_H
#define CPP_PILOT_HSE_ZIPPER_H

#include <fstream>
#include <string>
#include <vector>

#include "priority_queue.h"
#include "out_wrapper.h"
#include "to_trie.h"
#include "compare_codes.h"


class Zipper {
public:
    Zipper();

    void Zip(const std::vector<std::string>& args);

protected:
    static const size_t NUM_OF_SPECIAL_BITS;
    static const int16_t FILENAME_END;
    static const int16_t ONE_MORE_FILE;
    static const int16_t ARCHIVE_END;
    std::fstream out_file_;
    OutWrapper out_;
    ToTrie all_;
    std::fstream file_;
    std::unordered_map<int16_t, size_t> frequency_;
    void OpenInFile(std::fstream& in, const std::string& file_name);

    void OpenOutFile(std::fstream& out, const std::string& file_name);

    void WriteFileName(const std::string& file_name);

    void RetrieveFrequencyFromFileName(const std::string& file_name);

    void ZipFile(const std::string& file_name, bool end);

    void WriteFile(const std::string& file_name, bool end);

    void RetrieveFrequencyFromFile(const std::string& file_name);

    void WriteAlphabetAndCodes();

    void WriteFileContents(std::fstream& this_file);
};

#endif  // CPP_PILOT_HSE_ZIPPER_H
