#pragma once

#ifndef NODE_H
#define NODE_H

template <typename T>
struct Node { //for queue
    T data;
    Node<T>* next;

    Node(T data);

};

#include "Node.tpp"
#endif // !NODE_H
