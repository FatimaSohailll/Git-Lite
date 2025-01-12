#include "AVLNode.h"


    AVLNode:: AVLNode(fs::path directory) : height(0), parent("nullNode.txt"), leftChild("nullNode.txt"), rightChild("nullNode.txt"), directory(directory) {
    }

    AVLNode::AVLNode(str key, str parent, fs::path directory) :height(0), parent(parent), leftChild("nullNode.txt"), rightChild("nullNode.txt"), directory(directory) {
        this->key.value = key;
    }

    AVLNode::AVLNode(str key, str parent, int h, str l, str r, char* directory) :parent(parent), height(h), leftChild(l), rightChild(r), directory(directory) {
        this->key.value = key;
    }

    void AVLNode::readNode(str nodeFilePath) { //file to node
        /*
            key
            height
            left child
            right child
            parent path
            num Rec
            record
        */
        //if (nodeFilePath == "nullptr") {
        //	updateRoot(nullptr, "nullptr");
        //	return;
        //}
        //AVLNode* tmp = new AVLNode;
        const int LINE_LENGTH = 255;
        fs::path filePath = directory / nodeFilePath.c_str();
        fstream nodeFile;
        nodeFile.open(filePath);
        char line[LINE_LENGTH] = { 0 };
        if (!nodeFile.is_open())
            return;

        nodeFile.getline(line, LINE_LENGTH);
        this->key.value = line;
        nodeFile.getline(line, LINE_LENGTH);
        this->height = stoi(line);
        nodeFile.getline(line, LINE_LENGTH);
        this->leftChild = line;
        nodeFile.getline(line, LINE_LENGTH);
        this->rightChild = line;
        nodeFile.getline(line, LINE_LENGTH);
        this->parent = line;
        nodeFile.getline(line, LINE_LENGTH);
        int rec = stoi(line);
        for (int i = 0; i < rec; i++) {
            nodeFile.getline(line, LINE_LENGTH);
            this->key.duplicates.insert(line);
        }
        //updateRoot(tmp, nodeFilePath);
        nodeFile.close();
    }

    void AVLNode::writeNode(str nodePath) { //node to file
        fs::path filePath = directory / nodePath.c_str();
        ofstream nodeFile;
        nodeFile.open(filePath);
        nodeFile << this->key.value << "\n"; //key
        nodeFile << this->height << "\n";
        nodeFile << this->leftChild << "\n"; //left child (if have)
        nodeFile << this->rightChild << "\n"; //right child (if have)
        nodeFile << this->parent << "\n"; //right child (if have)
        int numInstances = this->key.duplicates.size;
        nodeFile << numInstances << "\n";
        for (int i = 0; i < numInstances; i++) {
            nodeFile << this->key.duplicates[i].line << '\n';
        }
        //this->record.writeAllRecords(nodeFile);
        nodeFile.close();
    }

    void AVLNode::updateHeight(str path) { //max(left child,right child)
        AVLNode* leftChild = new AVLNode(directory);
        leftChild->readNode(this->leftChild);
        AVLNode* rightChild = new AVLNode(directory);
        rightChild->readNode(this->rightChild);
        int leftCHeight = leftChild->height;
        int rightCHeight = rightChild->height;
        this->height = max(leftCHeight, rightCHeight) + 1;
        this->writeNode(path);
        delete leftChild;
        delete rightChild;
    }

    int AVLNode::balanceFactor() {
        AVLNode* leftChild = new AVLNode(directory);
        leftChild->readNode(this->leftChild);
        AVLNode* rightChild = new AVLNode(directory);
        rightChild->readNode(this->rightChild);
        int leftHeight = leftChild->height;
        int rightHeight = rightChild->height;
        delete leftChild;
        delete rightChild;
        return (rightHeight - leftHeight);
    }

    str AVLNode::checkRotation(str path) {
        str newRoot = path;
        if (this->balanceFactor() == -2) {
            AVLNode* leftC = new AVLNode(directory);
            leftC->readNode(this->leftChild);
            if (leftC->balanceFactor() == 1) {
                newRoot = doubleRightRotation(path);
            }
            else {
                //readInNode(rootP);
                newRoot = singleRightRotate(path);
                //readInNode(newRoot);
            }
            delete leftC;
        }
        else if (this->balanceFactor() == 2) {
            AVLNode* rightC = new AVLNode(directory);
            rightC->readNode(this->leftChild);
            if (rightC->balanceFactor() == -1) {
                newRoot = doubleLeftRotation(path);
            }
            else {
                //readInNode(rootP);
                newRoot = singleLeftRotate(path);
                //readInNode(newRoot);
            }
            delete rightC;
        }
        return newRoot;
    }

    str AVLNode::singleLeftRotate(str path) {

        AVLNode* k = new AVLNode(directory);
        str kPath = this->rightChild;
        k->readNode(kPath); //candidate

        this->rightChild = k->leftChild;
        k->leftChild = path;
        k->parent = this->parent;
        this->parent = kPath;

        this->writeNode(path);
        this->updateHeight(path);

        k->writeNode(kPath);
        k->updateHeight(kPath);
        delete k;

        return kPath;
    }

    str AVLNode::singleRightRotate(str path) {

        AVLNode* k = new AVLNode(directory);
        str kPath = this->leftChild; //candidate
        k->readNode(kPath);

        this->leftChild = k->rightChild;
        k->rightChild = path;
        k->parent = this->parent;
        this->parent = kPath;

        this->writeNode(path);
        this->updateHeight(path);

        k->writeNode(kPath);
        k->updateHeight(kPath);
        delete k;

        return kPath;
    }

    str AVLNode::doubleLeftRotation(str path) {

        AVLNode* rotChild = new AVLNode(directory);
        rotChild->readNode(this->rightChild);
        str midroot = rotChild->singleRightRotate(this->rightChild);
        this->rightChild = midroot;
        delete rotChild;

        str newRoot = this->singleLeftRotate(path);
        return newRoot;
    }
    str AVLNode::doubleRightRotation(str path) {

        AVLNode* rotChild = new AVLNode(directory);
        rotChild->readNode(this->leftChild);
        str midroot = rotChild->singleLeftRotate(this->leftChild);
        this->leftChild = midroot;
        delete rotChild;

        str newRoot = this->singleRightRotate(path);
        return newRoot;
    }

    str AVLNode::updateNodeFileName(str path) {
        // Generate new filename based on current node data
        char* newFileName = generateFileNameWithKey(this->key.value);
        if (path != newFileName) {
            /*fs::rename(path.c_str(), newFileName);*/
            try {
                fs::path filePath1 = directory / path.c_str();
                fs::path filePath2 = directory / newFileName;
                std::filesystem::rename(filePath1, filePath2);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Error: " << e.what() << '\n';
                std::cerr << "Code: " << e.code() << '\n';
            }

            //path = newFileName;
        }
        str newName = newFileName;
        delete[] newFileName;
        return newName;
    }
