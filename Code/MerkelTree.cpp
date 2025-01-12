#include "MerkelTree.h"

   
MerkelTree::  MerkelTree(AVLTree* avl, fs::path repository) :leafPaths("Layer0.txt") {
        //directory = repository.parent_path() / fs::path("merkle");  //idk the paths rn soo....
        directory = repository / fs::path("merkle");  //idk the paths rn soo....
        if (!fs::exists(directory))
            fs::create_directories(directory);
        this->repo = repository.c_str();
        nullNode = "nullNode.txt";
        fs::path filePath2 = directory / leafPaths.c_str();
        ofstream leafFile;
        leafFile.open(filePath2);
        buildMerkleTreeTraversalAVL(avl->rootPath, leafFile);
        leafFile.close();
        buildMerkleTree();
    }
    //MerkelTree(RedBlackTree& rb, fs:: path repository) :leafPaths("Layer0.txt") {
    //	directory = fs::path("MERKLETREES") / "RBTREE";   //idk the paths rn soo....
    //	treeDirectory = fs::path("RBTREES");
    //	if (!fs::exists(directory))
    //		fs::create_directories(directory);
    //	this->repo = repository;
    //	nullNode = "nullNode.txt";
    //	fs::path filePath2 = directory / leafPaths.c_str();
    //	ofstream leafFile;
    //	leafFile.open(filePath2);
    //	buildMerkleTreeTraversalRBtree(rb.rootPath, leafFile);
    //	leafFile.close();
    //	buildMerkleTree();
    //}
MerkelTree:: MerkelTree(BTree* btree, fs::path repository, int m) :m(m), leafPaths("Layer0.txt") {
        directory = repository / "BTREE";
        treeDirectory = fs::path("BTREES");
        if (!fs::exists(directory))
            fs::create_directories(directory);
        this->repo = repository;
        nullNode = "nullNode.txt";
        fs::path filePath2 = directory / leafPaths.c_str();
        ofstream leafFile;
        leafFile.open(filePath2);
        buildMerkleTreeTraversalBtree(btree->rootPath, leafFile);
        leafFile.close();
        buildMerkleTree();
    }
    void  MerkelTree:: CreateLeafHash(str& rootPath, ofstream& leafPaths) {
        leafPaths << rootPath << "\n";
        countLeafs++;
        SHA256 sha;
        fstream merkelfile;
        string hashString = "";
        int lineLength = 100;
        char line[100];
        str dataL;
        fs::path filePath1 = repo / rootPath.c_str();
        merkelfile.open(filePath1);
        while (merkelfile.getline(line, lineLength)) {
            //merkelfile.getline(line, lineLength);
            hashString += line;
        }
        merkelfile.close();
        char* fileName = generateleafFileName(countLeafs);
        fs::path filePath2 = directory / fileName;
        ofstream merkelFile2;
        merkelFile2.open(filePath2);
        sha.update(hashString);
        merkelFile2 << sha.final() << "\n";
        merkelFile2 << nullNode << "\n";
        merkelFile2 << nullNode << "\n";
        merkelFile2 << /*filePath1.string()*/ rootPath.c_str() << "\n";
        merkelFile2.close();
        merkelNode* merkleLeaf = new merkelNode(sha.final(), fileName);
        leaf.push_back(merkleLeaf);
    }
    void  MerkelTree:: buildMerkleTreeTraversalAVL(str& rootPath, ofstream& leafPaths) {
        AVLNode* root = new AVLNode(repo);
        root->readNode(rootPath);
        if (rootPath == nullNode) {
            return;
        }
        str leftPath = root->leftChild;
        str rightPath = root->rightChild;
        CreateLeafHash(rootPath, leafPaths);
        buildMerkleTreeTraversalAVL(leftPath, leafPaths);
        buildMerkleTreeTraversalAVL(rightPath, leafPaths);

    }
    //void buildMerkleTreeTraversalRBtree(str& rootPath, ofstream& leafPaths) {
    //	RedBlackTree* root = new RedBlackTree(repo);  //idk the paths rn soo....
    //	root->readInNode(rootPath);
    //	if (rootPath == nullNode) {
    //		return;
    //	}
    //	str leftPath = root->root->leftChild;
    //	str rightPath = root->root->rightChild;
    //	CreateLeafHash(rootPath, leafPaths);
    //	buildMerkleTreeTraversalRBtree(leftPath, leafPaths);
    //	buildMerkleTreeTraversalRBtree(rightPath, leafPaths);

    //}
    void  MerkelTree::buildMerkleTreeTraversalBtree(str& rootPath, ofstream& leafPaths) {
        BTreeNode* root = new BTreeNode(repo, m);
        root->readNode(rootPath);
        if (rootPath == nullNode) {
            return;
        }
        CreateLeafHash(rootPath, leafPaths);
        int numChild = root->m;
        for (int i = 0; i < m + 1; i++) {
            buildMerkleTreeTraversalBtree(root->child[i], leafPaths);
        }

    }
    void  MerkelTree:: buildMerkleTree() {
        ifstream leafFile;
        int maxLength = 100;
        char arr[25];
        vec<merkelNode*> Layer = leaf;
        int count = countLeafs;
        int layer = 1;
        int count2 = 0;
        while (count > 1) {
            vec<merkelNode*> newlayer;
            for (int i = 0; i < count; i += 2) {

                if (Layer.getSize() - 1 > i) {
                    SHA256 sha;
                    string hashString = Layer[i]->Hash + Layer[i + 1]->Hash;
                    char* fileName1 = generateLayerFileName(layer, i + 1);
                    fs::path filePath1 = directory / fileName1;
                    ofstream merkelfile;
                    sha.update(hashString);
                    merkelfile.open(filePath1);
                    merkelfile << sha.final() << "\n";
                    merkelfile << Layer[i]->thisPath << "\n";
                    merkelfile << Layer[i + 1]->thisPath << "\n";
                    merkelfile.close();
                    merkelNode* merkel = new merkelNode(sha.final(), fileName1, Layer[i]->thisPath, Layer[i + 1]->thisPath);
                    newlayer.push_back(merkel);
                    count2++;
                }
                else {
                    SHA256 sha;
                    string hashString = Layer[i]->Hash + Layer[i]->Hash;
                    char* fileName1 = generateLayerFileName(layer, i + 1);
                    fs::path filePath1 = directory / fileName1;
                    ofstream merkelfile;
                    sha.update(hashString);
                    merkelfile.open(filePath1);
                    merkelfile << sha.final() << "\n";
                    merkelfile << Layer[i]->thisPath << "\n";
                    merkelfile << Layer[i]->thisPath << "\n";
                    merkelfile.close();
                    merkelNode* merkel = new merkelNode(sha.final(), fileName1, Layer[i]->thisPath, Layer[i]->thisPath);
                    newlayer.push_back(merkel);
                    count2++;
                }
            }
            Layer = newlayer;
            layer++;
            count = count2;
            count2 = 0;
        }
        this->leaf = Layer;
    }
