#pragma once

#include "vec.h"

template <typename T>
vec<T>::vec() : capacity(10), size(0) {
    data = new T[capacity];
}

template <typename T>
vec<T>::vec(const vec& copy) : capacity(copy.capacity), size(copy.size) {
    data = new T[capacity];
    for (unsigned int i = 0; i < size; i++) {
        data[i] = copy.data[i];
    }
}

template <typename T>
vec<T>::~vec() {
    delete[] data;
}

template <typename T>
void vec<T>::resize() {
    capacity *= 2;
    T* newData = new T[capacity];
    for (unsigned int i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

template <typename T>
void vec<T>::push_back(const T& value) {
    if (size == capacity) {
        resize();
    }
    data[size++] = value;
}

template <typename T>
void vec<T>::pop_back() {
    if (size > 0) {
        --size;
    }
}

template <typename T>
void vec<T>::copy_back(T value) {
    if (size == capacity) {
        resize();
    }
    data[size++] = value;
}

template <typename T>
T& vec<T>::operator[](int index) {
    return data[index];
}

template <typename T>
void vec<T>::operator=(const vec& copy) {
    delete[] data; // Avoid memory leak
    capacity = copy.capacity;
    size = copy.size;
    data = new T[capacity];
    for (unsigned int i = 0; i < size; i++) {
        data[i] = copy.data[i];
    }
}

template <typename T>
unsigned int vec<T>::getSize() const {
    return size;
}

template <typename T>
void vec<T>::setSize(unsigned int newSize) {
    if (newSize > capacity) {
        resize();
    }
    size = newSize;
}

template <typename T>
void vec<T>::clear() {
    size = 0;
}
