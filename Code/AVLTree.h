#pragma once

#ifndef AVLTREE_H
#define AVLTREE_H

#include "AVLNode.h"

char* extractColumn(const char* line, int col);

class AVLTree {
public:
    fs::path directory;

    str rootPath;
    str nullNode;


    AVLTree(fs::path dir);
    str insert(str path, str key, str parent, str record);
    void insertNode(str key, str record);
    str deleteNode(str path, str key);
    void remove(str key);
    bool search(str key);
    bool search(str key, str column, int col);
    /// /// visualizing the tree -- uses queue
    //void visualize() {
    //	if (rootPath == "nullptr") {
    //		cout << "Tree is empty." << endl;
    //		return;
    //	}

    //	queue<str> q;
    //	q.push(rootPath);

    //	int maxLevel = getMaxLevel(rootPath);
    //	int maxWidth = (1 << maxLevel) - 1;

    //	while (!q.empty()) {
    //		int levelSize = q.size();
    //		int spaces = maxWidth / levelSize;
    //		while (levelSize > 0) {
    //			str nodePath = q.front();
    //			q.pop();
    //			readInNode(nodePath);
    //			cout << setw(spaces) << root->key << setw(spaces) << " ";

    //			if (root->leftChild != "nullptr") q.push(root->leftChild);
    //			if (root->rightChild != "nullptr") q.push(root->rightChild);

    //			levelSize--;
    //		}
    //		cout << endl;
    //	}
    //}

    str getFilePath(str key);
};

#endif // !AVLTREE_H
