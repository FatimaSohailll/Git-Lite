#include "RedBlackNode.h"

RedBlackNode:: RedBlackNode(fs::path directory) :key(0), color(1), leftChild("nullNode.txt"), rightChild("nullNode.txt"), parent("nullNode.txt"), record("empty") {}
RedBlackNode:: RedBlackNode(int key, str parent, fs::path directory, str record) :key(key), color(1),
        parent(parent), leftChild("nullNode.txt"), rightChild("nullNode.txt"), record(record) {}
RedBlackNode:: RedBlackNode(RedBlackNode& node) :key(node.key), color(node.color), leftChild(node.leftChild), rightChild(node.rightChild), parent(node.parent), directory(node.directory), record(node.record) {}