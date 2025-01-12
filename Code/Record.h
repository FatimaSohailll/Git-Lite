#pragma once

#ifndef RECORD_H
#define RECORD_H

#include "str.h"
#include "vec.h"

struct Record {
    int numColoumns;
    vec<str> coloumns;
    str line;
    //Record* next;
public:
    Record();
    Record(str l);
    void insertRecord(str l);
    str operator[](int index);
    void display();
};

#endif // !RECORD_H
