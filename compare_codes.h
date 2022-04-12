#pragma once

#ifndef ARCHIVER_COMPARECODES_H
#define ARCHIVER_COMPARECODES_H

#include <vector>

struct CompareCodes {
    bool operator()(const std::vector<bool> vec1, const std::vector<bool> vec2) const {
        if (vec1.size() == vec2.size()) {
            return vec1 < vec2;
        }
        return vec1.size() < vec2.size();
    }
};

#endif  // ARCHIVER_COMPARECODES_H
