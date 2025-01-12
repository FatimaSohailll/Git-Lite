#pragma once

#include "Node.h"

template <typename T>
    Node<T>:: Node(T data) :data(data), next(nullptr) {}
