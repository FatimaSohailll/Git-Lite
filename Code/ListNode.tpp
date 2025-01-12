#pragma once

#include "ListNode.h"

template <typename T>
   ListNode<T>:: ListNode() : next(nullptr) {}

template <typename T>
  ListNode<T>::  ListNode(T data) :data(data), next(nullptr) {}

template <typename T>
   ListNode<T>:: ListNode(const ListNode*& copy) {
        if (copy == nullptr) {
            // If the input node is null, this node should be initialized as an empty node.
            this->data = T(); // Default-initialize data.
            this->next = nullptr;
            return;
        }

        this->data = copy->data;
        ListNode<T>* current = this;
        const ListNode<T>* temp = copy->next;

        while (temp) {
            current->next = new ListNode<T>(temp->data);
            current = current->next;
            temp = temp->next;
        }
        current->next = nullptr;
    }

template <typename T>
    void ListNode<T>:: operator=(ListNode<T>*& copy) {
        ListNode<T> curr = copy;
        ListNode<T> temp = next;
        while (curr) {
            this->data = curr->data;
            temp = new ListNode<T>;
            temp = temp->next;
            curr = curr->next;
        }
    }