#pragma once

#ifndef QUEUE_H
#define QUEUE_H

#include"Node.h"

template <typename T>
class Queue {
public:
    Node<T>* front;
    Node<T>* rear;

    Queue();
    bool empty();
    void print();
    void enqueue(T data);
    T dequeue();
    T Front();
    int size();
    void clear();
};

#include "Queue.tpp"
#endif // !QUEUE_H
