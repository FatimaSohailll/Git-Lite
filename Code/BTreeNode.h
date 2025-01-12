#pragma once

#ifndef BTREENODE_H
#define BTREENODE_H

#include "Key.h"
#include "Filing.h"

#include<string>
#include<fstream>
#include <filesystem>
namespace fs = std::filesystem;

struct BTreeNode {
    fs::path directory;

    int m;
    Key* keys;  //m-1 but for full +1
    int numKey;
    str* child; //m but for full +1
    bool isLeaf;
    str parent;
    str nullNode;

    BTreeNode(fs::path dir, int m);
    void writeNode(str nodePath);
    void readNode(str nodePath);
    bool isFull();
    bool isDefecit();
    //display (for testing)
    void display();
    //search
    int search(str searchKey);
    //insert
    int insert(str newKey, str record);
    int insert(Key newKey);
    void remove(int k, bool removeAll);
    void clearNode();
    Key getSuccessor(int i);
    Key getPredecessor(int i);
    //split cth child of root, if its full
    void splitChild(str path, BTreeNode* borrowChild, int c, int& numNodes);
    void borrowFromLeft(str path, int c);
    void borrowFromRight(str path, int c);
    void merge(str path, int c);
    void fill(str path, int c);
    void removeFromLeaf(str path, int i, bool removeAll);
    void removeFromInternal(str path, int i);
    BTreeNode* removeNode(str path, str key, bool removeAll);
    ~BTreeNode();
};

#endif // !BTREENODE_H
