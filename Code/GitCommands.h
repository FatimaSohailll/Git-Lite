#pragma once

#ifndef GITCOMMANDS_H
#define GITCOMMANDS_H

#include "MerkelTree.h"
#include "RedBlackTree.h"
#include "SQL.h"

//CONSTANTS
const int MAX_QUERY_LENGTH = 500;
const int MAX_COMMIT_MSG_LENGTH = 100;
const int MAX_COMMAND_LENGTH = 100;
const int MAX_RECORD_LENGTH = 500;

void compareNodesAVL(str merkel1, str merkel2, fs::path main, fs::path branch);
void compareMerkelsAVL(str mainMerkel, str branchMerkel, fs::path main, fs::path branch);

//////////////////////////////////GIT COMMANDS

void querySession(fs::path dir);
bool hasBOM(ifstream& file);
Record getColumns(ifstream& dataFile);
char* extractColumn(const char* line, int col);
void loadData(ifstream& dataFile, BTree*& btree, int col);
void loadData(ifstream& dataFile, AVLTree*& avl, int col);
void loadData(ifstream& dataFile, RedBlackTree*& rbtree, int col);
void makeAVL(AVLTree* avl, fs::path directory, ifstream& dataFile, int hashAlg, int col);
void makeRedBlack(RedBlackTree* rbtree, fs::path directory, ifstream& dataFile, int hashAlg, int col);
void makeBTree(BTree* btree, fs::path directory, ifstream& dataFile, int hashAlg, int col);
void initRepository(fs::path& currDir);
void duplicateFolder(const fs::path& source, const fs::path& destination);

#endif // !GITCOMMANDS_H
