#pragma once

#ifndef FILING_H
#define FILING_H

#include "str.h"

const int MAX_FILENAME = 255;

char* generateNodeFileName(string blockPtr, int blockOffset);
char* generateFileNameWithKey(str key);
char* generateLayerFileName(int layerN, int i);
char* generateleafFileName(int numNodes);
void intToChars(int num, char*& str);
char* generateFileName(int numNodes);

#endif // !FILING_H
