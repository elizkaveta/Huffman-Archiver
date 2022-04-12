#ifndef CPP_PILOT_HSE_NODE_H
#define CPP_PILOT_HSE_NODE_H

#include <iostream>


class Node {
public:
    size_t frequency = 0;
    int16_t symbol = -1;
    Node* l = nullptr;
    Node* r = nullptr;

    Node(size_t freq, int16_t sym) {
        frequency = freq, symbol = sym;
    }

    Node(Node* left, Node* right) {
        frequency = left->frequency + right->frequency;
        l = left;
        r = right;
        symbol = -1;
    }

    ~Node() {
        delete r;
    }
};

#endif  // CPP_PILOT_HSE_NODE_H
