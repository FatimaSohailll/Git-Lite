#pragma once

#ifndef LISTNODE_H
#define LISTNODE_H

template <typename T>
struct ListNode {
    T data;
    ListNode* next;

    ListNode();
    ListNode(T data);
    ListNode(const ListNode*& copy);
    void operator=(ListNode<T>*& copy);
};

#include "ListNode.tpp"
#endif // !LISTNODE_H
