#ifndef CPP_PILOT_HSE_HEAP_H
#define CPP_PILOT_HSE_HEAP_H

#include <iostream>

#include "node.h"

class PriorityQueue {
public:
    void UniteTwoNodes();

    Node* ExtractMin();

    void InsertNode(Node* key);

    ~PriorityQueue();

    void GetTree();

    Node* GetRootAndTree();

protected:
    static const size_t MAX_NUM_OF_NODES = 1000;
    Node** nodes_ = new Node* [MAX_NUM_OF_NODES];
    size_t heap_size_ = 0;

    void ToDown(size_t i);

    void ToUp(size_t i);
};

#endif  // CPP_PILOT_HSE_HEAP_H
