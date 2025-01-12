#pragma once

#ifndef REDBLACKNODE_H
#define REDBLACKNODE_H

#include "str.h"
#include "Filing.h"

#include <string>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

struct RedBlackNode{
    fs::path directory;
    int key;
    int color; //0 black,1 red
    str leftChild;
    str rightChild;
    str parent;
    str record;
    RedBlackNode(fs::path directory);
    RedBlackNode(int key, str parent, fs::path directory, str record);
    RedBlackNode(RedBlackNode& node);
};

#endif // !REDBLACKNODE_H
