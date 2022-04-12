#ifndef CPP_PILOT_HSE_TRIE_H
#define CPP_PILOT_HSE_TRIE_H

#include <map>
#include <vector>
#include <unordered_map>

#include "node.h"
#include "to_trie.h"


class Trie {
public:
    Trie(std::unordered_map<int16_t, size_t> frequ);

    ~Trie();

    void ChangeCodesToCanonical();

    ToTrie GetCodeInfo() const;

protected:
    std::vector<bool> code_;
    std::unordered_map<int16_t, size_t> frequency_;
    std::map<std::vector<bool>, int16_t, CompareCodes> symbol_of_code_;
    std::unordered_map<int16_t, std::vector<bool> > canonical_codes_;
    std::vector<size_t> count_of_codes_with_that_size_;
    size_t max_code_size_ = 0;
    Node* root_;

    void Dfs(const Node* this_root);

    void GetNextCanonicalCode(std::vector<bool>& vec, size_t new_size) const;
};

#endif  // CPP_PILOT_HSE_TRIE_H
