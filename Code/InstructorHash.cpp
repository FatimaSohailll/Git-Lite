#include "InstructorHash.h"

//instructr hash
str multiplyStrings(str& num1, str& num2) {
    int len1 = num1.getSize();
    int len2 = num2.getSize();
    str result;
    for (int i = 0; i < len1 + len2; i++) {
        result.push_back('0');
    }

    for (int i = len1 - 1; i >= 0; i--) {
        for (int j = len2 - 1; j >= 0; j--) {
            int product = (num1[i] - '0') * (num2[j] - '0');
            int pos1 = i + j;
            int pos2 = i + j + 1;
            int sum = product + (result[pos2] - '0');

            result[pos2] = (sum % 10) + '0';
            result[pos1] = result[pos1] + (sum / 10);
        }
    }

    // Remove leading zeros
    str final;
    bool leadingZero = true;
    for (unsigned int i = 0; i < result.getSize(); i++) {
        if (leadingZero && result[i] != '0') {
            leadingZero = false;
        }
        if (!leadingZero) {
            final.push_back(result[i]);
        }
    }
    final.push_back('\0');
    return final.getSize() == 0 ? str("0") : final;
}

str moduloString(str& num, int m) {
    int result = 0;
    for (unsigned int i = 0; i < num.getSize(); i++) {
        result = (result * 10 + (num[i] - '0')) % m;
    }
    return toString(result);
}

str hashString(str& s) {
    str result("1");
    for (unsigned int i = 0; i < s.getSize(); i++) {
        str ascii = toString(static_cast<int>(s[i]));
        result = multiplyStrings(result, ascii);
    }
    return moduloString(result, 29);
}

str hashInteger(str& s) {
    str result("1");
    for (unsigned int i = 0; i < s.getSize(); i++) {
        str digit;
        digit.push_back(s[i]);
        result = multiplyStrings(result, digit);
    }
    return moduloString(result, 29);
}

bool isAllDigits(str& s) {
    for (unsigned int i = 0; i < s.getSize(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}

