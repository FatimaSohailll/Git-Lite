#pragma once

#ifndef KEY_H
#define KEY_H

#include"LinkedList.h"

struct Key {
    str value;
    LinkedList<Record> duplicates;

    Key();
    Key(int value);
    Key(const Key& copy);
    void operator=(const Key& other);
    void clearKey();
    void insertDuplicates(str record);
    bool operator==(const Key& other);
    ~Key();
};

ostream& operator<<(ostream& output, Key& key);

#endif // !KEY_H
