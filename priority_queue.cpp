#include "priority_queue.h"


PriorityQueue::~PriorityQueue() {
    delete[] nodes_;
}

void PriorityQueue::ToDown(size_t i) { // Спускает элемент вниз
    if (nodes_[i] == nullptr) {
        return;
    }
    while (2 * i + 1 < heap_size_) {
        size_t left_son = 2 * i + 1;
        size_t right_son = 2 * i + 2;
        size_t ind_of_min_of_sons_freq = left_son;
        if (right_son < heap_size_ && nodes_[right_son]->frequency < nodes_[left_son]->frequency) {
            ind_of_min_of_sons_freq = right_son;
        }
        if (nodes_[i]->frequency <= nodes_[ind_of_min_of_sons_freq]->frequency) {
            break;
        }
        std::swap(nodes_[i], nodes_[ind_of_min_of_sons_freq]);
        i = ind_of_min_of_sons_freq;
    }
}

void PriorityQueue::ToUp(size_t i) { // Поднимает элемент из конца кучи вверх
    size_t parent_index = static_cast<size_t>((static_cast<int16_t>(i) - 1) / 2);
    while (nodes_[i]->frequency < nodes_[parent_index]->frequency) {
        std::swap(nodes_[i], nodes_[parent_index]);
        i = parent_index;
        if (!i) {
            break;
        }
    }
}

Node* PriorityQueue::ExtractMin() { // Извлекает минимальный элемент
    Node* mini = nodes_[0];
    --heap_size_;
    nodes_[0] = nodes_[heap_size_];
    nodes_[heap_size_] = nullptr;
    ToDown(0);
    return mini;
}

void PriorityQueue::InsertNode(Node* key) { // Вставляет Node
    nodes_[heap_size_] = key;
    ToUp(heap_size_);
    ++heap_size_;
}

void PriorityQueue::UniteTwoNodes() { // Объединяет две вершины, создавая предка
    Node* one = ExtractMin();
    Node* two = ExtractMin();
    Node* parent = new Node(one, two);
    InsertNode(parent);
}

void PriorityQueue::GetTree() { // Строит дерево
    while (heap_size_ > 1) {
        UniteTwoNodes();
    }
}

Node* PriorityQueue::GetRootAndTree() { // Выдает корень дерева
    GetTree();
    return ExtractMin();
}
