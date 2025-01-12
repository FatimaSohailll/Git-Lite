#pragma once

#ifndef AVLNODE_H
#define AVLNODE_H

#include<string>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "Key.h"
#include "Filing.h"

struct AVLNode {

    fs::path directory;

    Key key;
    int height;
    str leftChild;
    str rightChild;
    str parent;

    AVLNode(fs::path directory);
    AVLNode(str key, str parent, fs::path directory);
    AVLNode(str key, str parent, int h, str l, str r, char* directory);
    void readNode(str nodeFilePath);
    void writeNode(str nodePath);
    void updateHeight(str path);
    int balanceFactor();
    str checkRotation(str path);
    str singleLeftRotate(str path);
    str singleRightRotate(str path);
    str doubleLeftRotation(str path);
    str doubleRightRotation(str path);
    str updateNodeFileName(str path);
};
#endif // !AVLNODE_H
