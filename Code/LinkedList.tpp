#pragma once

#include "LinkedList.h"


template <typename T>
   LinkedList<T>:: LinkedList() :head(nullptr), size(0) {}

   template <typename T>
    void LinkedList<T>::insert(str data) {
        ListNode<Record>* curr = head;
        /*	while (curr && curr->next)
                curr = curr->next;*/
        if (curr == nullptr) {
            head = new ListNode<Record>;
            head->data.insertRecord(data);
        }
        else
        {
            ListNode<Record>* temp = curr;
            head = new ListNode<Record>;
            head->data.insertRecord(data);
            head->next = temp;
        }
        size++;
    }

    template <typename T>
    void LinkedList<T>::insert(T data) {
        ListNode<T>* newNode = new ListNode<T>(data);
        newNode->next = head;
        head = newNode;
        size++;
    }

    template <typename T>
    void LinkedList<T>:: remove() { //remove head
        if (head == nullptr)
            return;
        ListNode<T>* del = head;
        head = head->next;
        delete del;
        size--;
    }

    template <typename T>
    void LinkedList<T>::setHead(ListNode<T>*& r) {
        if (head) {
            delete head;
            head = nullptr;
        }
        head = r;
    }

    template <typename T>
    T LinkedList<T>::operator[](int i) {
        int index = 0;
        ListNode<T>* curr = head;
        while (curr && index < i) {
            curr = curr->next;
            index++;
        }
        if (curr)
            return curr->data;
        else
            return T();
    }

    template <typename T>
    void LinkedList<T>::writeAllRecords(ofstream& nodeFile) {
        ListNode<Record>* curr = head;
        while (curr) {
            nodeFile << curr->data.line << "\n";
            curr = curr->next;
        }
    }

    template <typename T>
    ListNode<Record>*& LinkedList<T>::getHead() {
        return head;
    }

    template <typename T>
    void LinkedList<T>:: clearList() {

        ListNode<T>* curr = head;
        while (curr) {
            ListNode<T>* tmp = curr;
            curr = curr->next;
            delete tmp;
        }

        head = nullptr;
        size = 0;

    }

    template <typename T>
    LinkedList<T>:: ~LinkedList() {
        ListNode<T>* curr = head;
        while (curr) {
            ListNode<T>* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
    }