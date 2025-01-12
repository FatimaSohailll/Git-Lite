#pragma once

#ifndef BTREE_H
#define BTREE_H

#include "BTreeNode.h"

class BTree {
public:
    //fs::path repo;
    fs::path directory;
    //char* column;
    str rootPath;
    int m; //m way
    str nullNode; //nullNode.txt for nullptrs
    int numNodes; //this is just for file naming!!


    BTree(fs::path dir, int m);
    //insert
    void insertNode(str newKey, str record);
    BTreeNode* insert(str path, str newKey, str record);
    //deletion
    void deleteKey(str delKey);
    //searching
    bool searchKey(str key);

};


#endif // !BTREE_H
