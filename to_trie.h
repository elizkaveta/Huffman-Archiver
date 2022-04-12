#ifndef ARCHIVER_TOTRIE_H
#define ARCHIVER_TOTRIE_H

#include <fstream>
#include <map>
#include <vector>
#include <unordered_map>

#include "compare_codes.h"


struct ToTrie {
    std::map<std::vector<bool>, int16_t, CompareCodes> symbol_of_code;
    std::unordered_map<int16_t, std::vector<bool> > canonical_forms;
    std::vector<size_t> count_of_codes_with_that_size;
    size_t max_code_size = 0;

    void Clear() {
        symbol_of_code.clear();
        canonical_forms.clear();
        count_of_codes_with_that_size.clear();
        max_code_size = 0;
    }
};

#endif  // ARCHIVER_TOTRIE_H
