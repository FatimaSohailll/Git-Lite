#pragma once
#ifndef MERKELNODE_H
#define MERKELNODE_H

#include "str.h"
#include<string>
#include<fstream>
#include <filesystem>
namespace fs = std::filesystem;

struct merkelNode {
    str thisPath;
    string Hash;
    str Path1;
    str Path2;

    merkelNode();
    merkelNode(string hash1, str path1);
    merkelNode(string hash, str  path1, str a1, str a2);
    bool isLeaf();
    void readNode(str path1, fs::path dir);
    void readNode(str path1);
    void WriteFile(fs::path path2);
};
#endif // !MERKELNODE_H
