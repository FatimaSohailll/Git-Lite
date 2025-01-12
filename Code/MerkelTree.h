#pragma once
#ifndef MERKELTREE_H
#define MERKELTREE_H

#include "MerkelNode.h"
#include "AVLTree.h"
#include "BTree.h"
#include "SHA256.h"

class MerkelTree {
public:
    vec<merkelNode*> leaf;
    string leafPaths;
    int countLeafs;
    fs::path directory;
    fs::path treeDirectory;
    fs::path repo;
    str nullNode;
    int m; //for btree
    MerkelTree(AVLTree* avl, fs::path repository);
    //MerkelTree(RedBlackTree& rb, fs:: path repository) ;
    MerkelTree(BTree* btree, fs::path repository, int m);
    void CreateLeafHash(str& rootPath, ofstream& leafPaths);
    void buildMerkleTreeTraversalAVL(str& rootPath, ofstream& leafPaths);
    //void buildMerkleTreeTraversalRBtree(str& rootPath, ofstream& leafPaths) ;
    void buildMerkleTreeTraversalBtree(str& rootPath, ofstream& leafPaths);
    void buildMerkleTree();
};
#endif // !MERKELTREE_H
