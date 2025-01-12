#pragma once
#ifndef SQL_H
#define SQL_H

#include "str.h"
#include "vec.h"
#include "AVLTree.h"
#include "BTree.h"
#include "InstructorHash.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
namespace fs = std::filesystem;
using namespace std;

class SQL {
public:
    str headingsCSV;
    vec<str> Titles;
    str repo;

    SQL(str headingsCSV, str repo1 = "hahaha");
    void parseHeadings(str& headingCSV, vec<str>& Titles);
    void executeQuery(vec<str>& query);
    void select(vec<str>& query);
    void update(vec<str>& query);
    void AVLupdatethisForThat(fs::path filename, str& condition, str& newthis, int conditioncol, int thiscol);
    void BTupdatethisForThat(fs::path filename, str& condition, str& newthis, int conditioncol, int thiscol);
    void Delete(vec<str>& query);
    void insert(vec<str>& query);
};

#endif // !SQL_H
