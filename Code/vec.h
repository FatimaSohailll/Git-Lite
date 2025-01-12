#pragma once
#ifndef VEC_H
#define VEC_H

template <typename T>
class vec {
private:
    T* data;
    unsigned int capacity;
    unsigned int size;

    void resize();

public:
    vec();
    vec(const vec& copy);

    ~vec();

    void push_back(const T& value);
    void pop_back();
    void copy_back(T value);
    T& operator[](int index);
    void operator=(const vec& copy);

    unsigned int getSize() const;
    void setSize(unsigned int size);

    void clear();
};

#include "vec.tpp"
#endif 