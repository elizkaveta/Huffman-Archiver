#include "trie.h"

#include "priority_queue.h"


// Строит дерево, обходит его
Trie::Trie(std::unordered_map<int16_t, size_t> frequ) : frequency_(frequ) {
    PriorityQueue que;
    for (auto&[symbol, freq]: frequency_) {
        Node* add = new Node(freq, symbol);
        que.InsertNode(add);
    }
    root_ = que.GetRootAndTree();
    Dfs(root_);
}

void Trie::Dfs(const Node* this_root) { // Обходит дерево
    if (this_root->symbol != -1) {
        symbol_of_code_[code_] = this_root->symbol;
        return;
    } else {
        code_.push_back(false);
        if (this_root->l != nullptr) {
            Dfs(this_root->l);
        }
        code_.back() = true;
        if (this_root->r != nullptr) {
            Dfs(this_root->r);
        }
        code_.pop_back();
    }
}

// Получает следующий канонический код
void Trie::GetNextCanonicalCode(std::vector<bool>& vec, size_t new_size) const {
    for (size_t i = vec.size() - 1; i >= 0; --i) {
        vec[i] = 1 - vec[i];
        if (vec[i]) {
            break;
        }
        if (i == 0) {
            vec.resize(vec.size() + 1);
            vec.assign(vec.size() + 1, false);
            vec[0] = true;
            break;
        }
    }
    vec.resize(std::max(vec.size(), new_size));
}

// Меняет код на канонический
void Trie::ChangeCodesToCanonical() {
    std::vector<bool> canonical_code;
    for (const auto& code_and_symbol : symbol_of_code_) {
        size_t code_size = code_and_symbol.first.size();
        int16_t symbol = code_and_symbol.second;
        if (canonical_code.empty()) {
            canonical_code.resize(code_size);
        } else {
            GetNextCanonicalCode(canonical_code, code_size);
        }
        canonical_codes_[symbol] = canonical_code;
        while (canonical_code.size() + 1 != count_of_codes_with_that_size_.size()) {
            count_of_codes_with_that_size_.push_back(0);
        }
        ++count_of_codes_with_that_size_[canonical_code.size()];
        max_code_size_ = std::max(max_code_size_, code_size);
    }
    symbol_of_code_.clear();
    for (auto& [symbol, code] : canonical_codes_) {
        symbol_of_code_[code] = symbol;
    }
}

ToTrie Trie::GetCodeInfo() const {
    return {symbol_of_code_, canonical_codes_,
            count_of_codes_with_that_size_, max_code_size_};
}
Trie::~Trie() {
    delete root_;
}
