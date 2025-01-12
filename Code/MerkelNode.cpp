#include "MerkelNode.h"

merkelNode:: merkelNode() {}
merkelNode:: merkelNode(string hash1, str path1) : thisPath(path1), Hash(hash1), Path1("nullptr"), Path2("nullptr") {}
merkelNode:: merkelNode(string hash, str  path1, str a1, str a2) :thisPath(path1), Hash(hash), Path1(a1), Path2(a2) {
}
bool merkelNode::isLeaf() {
    return (Path1 == "nullNode.txt" && Path2 == "nullNode.txt");
}
void merkelNode::readNode(str path1, fs::path dir) {
    thisPath = path1;
    fs::path fileNode = dir / path1.c_str();
    fstream merkelFile;
    merkelFile.open(fileNode);
    char line[100];
    int maxLineLength = 100;
    merkelFile.getline(line, maxLineLength);
    Hash = line;
    merkelFile.getline(line, maxLineLength);
    Path1 = line;
    merkelFile.getline(line, maxLineLength);
    Path2 = line;
    merkelFile.close();
}
void merkelNode:: readNode(str path1) {
    thisPath = path1;
    fs::path fileNode = path1.c_str();
    fstream merkelFile;
    merkelFile.open(fileNode);
    char line[100];
    int maxLineLength = 100;
    merkelFile.getline(line, maxLineLength);
    Hash = line;
    merkelFile.getline(line, maxLineLength);
    Path1 = line;
    merkelFile.getline(line, maxLineLength);
    Path2 = line;
    merkelFile.close();
}
void merkelNode:: WriteFile(fs::path path2) {
    fstream merkelFile;
    char line[100];
    int maxLineLength = 100;
    merkelFile.getline(line, maxLineLength);
    Hash = line;
    merkelFile.getline(line, maxLineLength);
    Path1 = line;
    merkelFile.getline(line, maxLineLength);
    Path2 = line;
    merkelFile.close();
}