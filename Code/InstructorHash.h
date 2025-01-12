#pragma once

#ifndef INSTRUCTORHASH_H
#define INSTRUCTORHASH_H

#include "str.h"

//instructr hash
str multiplyStrings(str& num1, str& num2);
str moduloString(str& num, int m);
str hashString(str& s);
str hashInteger(str& s);
bool isAllDigits(str& s);

#endif // !INSTRUCTORHASH_H
