#pragma once

#ifndef STRING_H
#define STRING_H

#include<iostream>
using namespace std;

class str {
private:
    unsigned int capacity;

    void resize();
public:
    char* data;
    unsigned int size;

    unsigned int string_length(const char* s) const;
    str();
    str(int capacity);
    str(const char* s);
    str(const str& other);
    str& operator=(const str& other);
    ~str();
    void push_back(const char& value);
    bool rfind(const char* subString, int start);
    char& operator[](int index);
    unsigned int getSize() const;
    void clear();
    const char* c_str() const;
    void append(const str& other);
    void operator +=(char chr);
    bool operator==(const char* other) const;
    bool operator==(const str& other) const;
    bool operator!=(const str& other) const;
    str operator+(const str& other) const;

    //comparison
    bool operator>(str str2);
    bool operator<(str str2);
    int rfind(const char* subString, int start, int end);
    char* substr(int start);

    friend std::ostream& operator<<(std::ostream& os, const str& s);
    friend std::istream& operator>>(std::istream& is, str& s);

};

//getline
std::istream& getline(std::istream& is, str& s);

//getline till space 
std::istream& getline(std::istream& is, str& s, char delim);

void string_copy(char* dest, const char* src);
int string2int(char* str);
unsigned int cstring_len(const char* s);

//stoi
int Stoi(const str& s);
str toString(int value);
str charToStr(const char* source);

#endif // !STRING_H
