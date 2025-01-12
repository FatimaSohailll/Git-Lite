#pragma once

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "ListNode.h"
#include "Record.h"

template <typename T>
class LinkedList {
public:
    ListNode<T>* head;
    int size;

    LinkedList();

    void insert(str data);
    void insert(T data);
    void remove();
    void setHead(ListNode<T>*& r);
    T operator[](int i);
    void writeAllRecords(ofstream& nodeFile);
    ListNode<Record>*& getHead();
    void clearList();
    ~LinkedList();
};

#include "LinkedList.tpp"
#endif // !LINKEDLIST_H
