#include "str.h"

void string_copy(char* dest, const char* src) {
    unsigned int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        ++i;
    }
    dest[i] = '\0';
}

int string2int(char* str) {
    int res = 0;
    int i = 0;
    while (str[i] != 0) {
        res *= 10;
        res += str[i] - 48;
        i++;
    }
    return res;
}


unsigned int cstring_len(const char* s) {
    unsigned int length = 0;
    while (s[length] != '\0') {
        ++length;
    }
    return length;
}
void str::resize() {
    capacity *= 2;
    char* newData = new char[capacity];
    for (unsigned int i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    for (int i = size; i < capacity; i++)
        newData[i] = 0;
    delete[] data;
    data = newData;
}

unsigned int str :: string_length(const char* s) const {
    unsigned int length = 0;
    while (s[length] != '\0') {
        ++length;
    }
    return length;
}


str::str() : capacity(10), size(0) {
    data = new char[capacity];
    for (int i = 0; i < capacity; i++)
        data[i] = 0;
}

str::str(int capacity) :capacity(capacity), size(0) {
    data = new char[capacity];
    for (int i = 0; i < capacity; i++)
        data[i] = 0;
}

str::str(const char* s) : size(string_length(s)), capacity(string_length(s) + 10) {
    data = new char[capacity];
    string_copy(data, s);
}

str::str(const str& other) : size(other.size), capacity(other.capacity) {
    data = new char[capacity];
    string_copy(data, other.data);
}

str& str ::operator=(const str& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new char[capacity];
        string_copy(data, other.data);
    }
    return *this;
}

str ::~str() {
    delete[] data;
}

void str::push_back(const char& value) {
    if (size == capacity) {
        resize();
    }
    data[size++] = value;
}
bool str::rfind(const char* subString, int start) {
    for (int i = start, j = 0; i < size; j++, i++) {
        if (data[i] != subString[j]) {
            return 1;
        }
    }
    return 0;
}
char& str :: operator[](int index) {
    return data[index];
}

unsigned int str::getSize() const {
    return size;
}

void str::clear() {
    size = 0;
}

const char* str::c_str() const {
    return data;
}

void str::append(const str& other) {
    for (unsigned int i = 0; i < other.size; ++i) {
        push_back(other.data[i]);
    }
}

void str ::operator +=(char chr) {
    push_back(chr);
}

bool str ::operator==(const char* other) const {
    int len = this->getSize();
    if (len == 0 && other[0] == '\0') {
        return true;
    }
    for (int i = 0; i < len; i++) {
        if (this->data[i] != other[i]) {
            return false;
        }
    }
    if (other[len] != '\0') {
        return false;
    }

    return true;
}

bool str ::operator==(const str& other) const {
    if (this->getSize() != other.getSize())
        return false;
    int len = this->getSize();
    for (int i = 0; i < len; i++) {
        if (this->data[i] != other.data[i])
            return false;
    }
    return true;
}

bool str ::operator!=(const str& other) const {
    return (!(*this == other));
}

str str ::operator+(const str& other) const {
    str result = *this;
    result.append(other);
    return result;
}


//comparison
bool str ::operator>(str str2) {
    if (this->size > str2.size)
        return true;
    if (this->size < str2.size)
        return false;
    for (int i = 0; i < size; i++) {
        if (this->data[i] < str2.data[i])
            return false; //less
        else if (this->data[i] > str2.data[i])
            return true; //greater
    }
    return false; //equal
}

bool str ::operator<(str str2) {
    if (this->size > str2.size)
        return false;
    if (this->size < str2.size)
        return true;
    for (int i = 0; i < size; i++) {
        if (this->data[i] < str2.data[i])
            return true; //less
        else if (this->data[i] > str2.data[i])
            return false; //greater
    }
    return false; //equal
}

int str::rfind(const char* subString, int start, int end) {
    int subLength = string_length(subString);
    if (start < 0 || end > size || start > end || subLength == 0) {
        return -1; // Invalid 
    }
    for (int i = end - subLength; i >= start; i--) {
        bool match = true;
        for (int j = 0; j < subLength; j++) {
            if (data[i + j] != subString[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return i;
        }
    }
    return-1;
}
char* str::substr(int start) {
    char* newName = new char[25] {0};
    unsigned int i = 0;
    while (data[start] != '\0') {
        newName[i] = data[start];
        ++i;
        start++;
    }
    return newName;
}

std::ostream& operator<<(std::ostream& os, const str& s) {
    for (unsigned int i = 0; i < s.size; ++i) {
        os << s.data[i];
    }
    return os;
}
//cin
std::istream& operator>>(std::istream& is, str& s) {
    char c;
    s.clear();
    while (is.get(c) && c != '\n') {
        s.push_back(c);
    }
    return is;
}
//getline
std::istream& getline(std::istream& is, str& s) {
    char c;
    s.clear();
    while (is.get(c) && c != '\n') {
        s.push_back(c);
    }
    return is;
}
//getline till space 
std::istream& getline(std::istream& is, str& s, char delim) {
    char c;
    s.clear();
    while (is.get(c) && c != delim && c != '\n') {
        s.push_back(c);
    }
    return is;
}
//stoi
int Stoi(const str& s) {
    int result = 0;
    for (unsigned int i = 0; i < s.getSize(); i++) {
        result = result * 10 + (s.data[i] - '0');
    }
    return result;
}

str toString(int value) {
    str result;
    while (value > 0) {
        result.push_back((value % 10) + '0');
        value /= 10;
    }
    for (unsigned int i = 0; i < result.getSize() / 2; i++) {
        char temp = result[i];
        result[i] = result[result.getSize() - i - 1];
        result[result.getSize() - i - 1] = temp;
    }
    return result;
}


str charToStr(const char* source) {
    if (!source) {
        return str("");
    }

    str result;
    unsigned int i = 0;
    while (source[i] != '\0') {
        result.push_back(source[i]);
        i++;
    }
    result.push_back('\0');
    result.size--;

    return result;
}