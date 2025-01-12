#pragma once
#include "Queue.h"
template <typename T>
     Queue<T>:: Queue() :front(nullptr), rear(nullptr) {}

     template <typename T>
    bool Queue<T>:: empty() {
        return (front == nullptr);
    }

    template <typename T>
    void Queue<T>:: print() {
        Node<T>* curr = front;
        while (curr) {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << "\n";
    }

    template <typename T>
    void Queue<T>::enqueue(T data) {
        if (rear) {
            rear->next = new Node<T>(data);
            rear = rear->next;
        }
        else { //first node
            front = new Node<T>(data);
            rear = front;
        }
    }

    template <typename T>
    T Queue<T>::dequeue() {
        if (empty()) {
            return T(); //empty
        }
        T tmp = front->data;
        Node<T>* toDel = front;
        front = front->next;
        delete toDel;
        if (front == nullptr)
            rear = nullptr;
        return tmp;
    }

    template <typename T>
    T Queue<T>::Front() { //peek
        if (front)
            return front->data;
        else
            return -1;
    }

    template <typename T>
    int Queue<T>::size() {
        int size = 0;
        Node<T>* curr = front;
        while (curr) {
            size++;
            curr = curr->next;
        }
        return size;
    }

    template <typename T>
    void Queue<T>::clear() {
        while (!empty())
            dequeue();
    }