#pragma once
#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "RedBlackNode.h"

class RedBlackTree {
public:
    fs::path directory;
    int nodeCount;
    RedBlackNode* root;
    str treeRoot;
    str rootPath;
    str parentPath;
    str grandparentPath;
    str greatGrandparentPath;
    str nullNode; //path to nullNode file 

    RedBlackTree(fs::path directory);
    void updateRoot(RedBlackNode* newRoot, str newRootPath);
    int getColour(str nodePath);
    int getParentColour();
    int getUncleAndColour(str& unclePath);
    void restoreRoot();
    void updateTreeRoot(str newRoot);
    void readInNode(str nodeFilePath);
    void writeNodeFile();
    //helpers for reorient CAN GET RID OF THESE IF YOU USE < >
    str getLeftChild(str nodePath);
    str getParentLeft();
    str getGrandparentLeft();
    str getGreatGrandparentLeft();
    //recolouring
    void singleRotRecolour();
    void doubleRotRecolour();
    //get keys (instead of ^^^)
    int getKey(str nodePath);
    void reOrient();
    void insert(int key, str record);
    //rotations (from avl)
    str doubleLeftRotation(str& rootP);
    str doubleRightRotation(str& rootP);
    str singleLeftRotate();
    str singleRightRotate();
    str search(int val);
    str getSuccessor(str& before);
    void fixDoubleBlack(str x);
    void removeNode(str& toDelete);
    void deleteKey(int val);
    void inOrderTraversal(str path);
};

#endif // !REDBLACKTREE_H
