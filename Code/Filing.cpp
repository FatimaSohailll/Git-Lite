#include "Filing.h"

void intToChars(int num, char*& str) {
    if (str)
        delete[] str;
    str = nullptr;
    int len = 0;
    while (num > 0) {
        int digit = num % 10;
        num /= 10;
        char* tmp = new char[len + 1];
        int j = len;
        for (int i = len - 1; i >= 0; i--) {
            tmp[j] = str[i];
            j--;
        }
        tmp[0] = digit + 48;
        delete[] str;
        str = tmp;
        len++;
    }
    char* tmp = new char[len + 1];
    for (int i = 0; i < len; i++)
        tmp[i] = str[i];
    tmp[len] = 0;
    delete[] str;
    str = tmp;
}

char* generateFileName(int numNodes) { //of first key
    char* filename = new char[MAX_FILENAME];
    for (int i = 0; i < 20; i++)
        filename[i] = 0;
    filename[0] = 'n';
    filename[1] = 'o';
    filename[2] = 'd';
    filename[3] = 'e';
    filename[4] = '-';
    int index = 5;
    char* nodeNum = nullptr;
    intToChars(numNodes + 1, nodeNum);
    int j = 0;
    while (nodeNum[j] != 0) {
        filename[index++] = nodeNum[j];
        j++;
    }
    filename[index++] = '.';
    filename[index++] = 't';
    filename[index++] = 'x';
    filename[index++] = 't';
    return filename;
}

char* generateNodeFileName(string blockPtr, int blockOffset) {
    char* filename = new char[MAX_FILENAME];
    for (int i = 0; i < 20; i++)
        filename[i] = 0;
    int index = 0;
    //get block#
    int j = 0;
    while (!(blockPtr[j] >= '1' && blockPtr[j] <= '9'))
        j++;
    while (blockPtr[j] != '.') {
        filename[index++] = blockPtr[j];
        j++;
    }
    //and then offset
    filename[index++] = '-';
    char* offset = nullptr;
    intToChars(blockOffset, offset);
    j = 0;
    while (offset[j] != 0) {
        filename[index++] = offset[j];
        j++;
    }
    filename[index++] = '.';
    filename[index++] = 't';
    filename[index++] = 'x';
    filename[index++] = 't';
    return filename;
}

char* generateFileNameWithKey(str key) {
    char* filename = new char[MAX_FILENAME];
    for (int i = 0; i < 100; i++)
        filename[i] = 0;
    filename[0] = 'n';
    filename[1] = '-';
    int index = 2;
    //char* nodeNum = nullptr;
    //intToChars(key, nodeNum);
    int j = 0;
    while (key[j] != 0) {
        filename[index++] = key[j];
        j++;
    }
    filename[index++] = '.';
    filename[index++] = 't';
    filename[index++] = 'x';
    filename[index++] = 't';
    return filename;
}

char* generateLayerFileName(int layerN, int i) { //of first key
    char* filename = new char[20];
    for (int i = 0; i < 20; i++)
        filename[i] = 0;
    filename[0] = 'l';
    filename[1] = 'a';
    filename[2] = 'y';
    filename[3] = 'e';
    filename[4] = 'r';
    filename[5] = layerN + 48;
    filename[6] = '-';
    int index = 7;
    char* nodeNum = nullptr;
    intToChars(i, nodeNum);
    int j = 0;
    while (nodeNum[j] != 0) {
        filename[index++] = nodeNum[j];
        j++;
    }
    filename[index++] = '.';
    filename[index++] = 't';
    filename[index++] = 'x';
    filename[index++] = 't';
    return filename;
}
char* generateleafFileName(int numNodes) { //of first key
    char* filename = new char[20];
    for (int i = 0; i < 20; i++)
        filename[i] = 0;
    filename[0] = 'l';
    filename[1] = 'e';
    filename[2] = 'a';
    filename[3] = 'f';
    filename[4] = '-';
    int index = 5;
    char* nodeNum = nullptr;
    intToChars(numNodes, nodeNum);
    int j = 0;
    while (nodeNum[j] != 0) {
        filename[index++] = nodeNum[j];
        j++;
    }
    filename[index++] = '.';
    filename[index++] = 't';
    filename[index++] = 'x';
    filename[index++] = 't';
    return filename;
}