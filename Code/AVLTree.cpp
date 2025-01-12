#include "AVLTree.h"


    AVLTree:: AVLTree(fs::path dir) {
        this->directory = dir;
        if (!fs::exists(directory))
            fs::create_directories(directory);
        //create nullnode
        nullNode = "nullNode.txt";
        fs::path filePath = directory / nullNode.c_str();
        fstream nullFile;
        nullFile.open(filePath);
        AVLNode* null = new AVLNode(directory); //null node
        null->height = -1;
        null->writeNode(nullNode);
        nullFile.close();
        delete null;
        rootPath = nullNode; //empty tree
    }

    str AVLTree::insert(str path, str key, str parent, str record) {  //recursive
        if (path == nullNode) {
            AVLNode* newNode = new AVLNode(key, parent, directory);
            char* filename = generateFileNameWithKey(key);
            newNode->key.insertDuplicates(record);
            newNode->writeNode(filename);
            //this->rootPath = filename;
            return filename;
        }
        else {
            AVLNode* root = new AVLNode(directory);
            root->readNode(path);
            if (key == root->key.value) {
                root->key.insertDuplicates(record);
                root->writeNode(path);
                return path;
            }
            else if (key < root->key.value) {
                str left = insert(root->leftChild, key, path, record);
                root->leftChild = left;
            }
            else {
                str right = insert(root->rightChild, key, path, record);
                root->rightChild = right;
            }
            str tmp = root->checkRotation(path);
            root->updateHeight(path);
            root->writeNode(path);
            delete root;
            return tmp;
        }
        //return checkRotation();	
    }

    void AVLTree::insertNode(str key, str record) {
        this->rootPath = insert(rootPath, key, nullNode, record);
    }


    str AVLTree::deleteNode(str path, str key) {

        if (path == nullNode)
            return nullNode;

        AVLNode* root = new AVLNode(directory);
        root->readNode(path);

        if (key < root->key.value) {	///lesser
            str left = deleteNode(root->leftChild, key);
            root->leftChild = left;
            //root->checkRotation(path);
            root->writeNode(path);
        }
        else if (key > root->key.value) {	///greater
            str right = deleteNode(root->rightChild, key);
            root->rightChild = right;
            //root->checkRotation(path);
            root->writeNode(path);
        }
        else {///equal
            if (root->key.duplicates.size > 1) { //just remov instance
                root->key.duplicates.remove();
                root->writeNode(path);
                return path;
            }
            //// 1 No children
            else if (root->leftChild == nullNode && root->rightChild == nullNode) {
                str parentPath = root->parent;
                if (parentPath != nullNode) {
                    AVLNode* parent = new AVLNode(directory);
                    parent->readNode(parentPath);
                    if (parent->leftChild == path) {
                        parent->leftChild = nullNode;
                    }
                    else {
                        parent->rightChild = nullNode;
                    }
                    //root->record.
                    parent->writeNode(parentPath);
                    delete parent;
                }
                fs::path filePath = directory / path.c_str();
                fs::remove(filePath);
                path = nullNode;
                //updateRoot(nullptr, "nullptr");

            }
            // 2 one child (right)
            else if (root->leftChild == nullNode) {

                str parentPath = root->parent;
                str rightChildPath = root->rightChild;

                AVLNode* rightChild = new AVLNode(directory);
                rightChild->readNode(rightChildPath);

                rightChild->parent = parentPath; //update parent of child
                rightChild->writeNode(rightChildPath);

                if (parentPath != nullNode) {		//update child of parent
                    AVLNode* parent = new AVLNode(directory);
                    parent->readNode(root->parent);

                    if (parent->leftChild == path) {
                        parent->leftChild = rightChildPath;
                    }
                    else {
                        parent->rightChild = rightChildPath;
                    }
                    parent->writeNode(parentPath);
                    delete parent;
                }
                fs::path filePath = directory / path.c_str();
                fs::remove(filePath);
                delete rightChild;
                path = rightChildPath;
                //readInNode(rightChildPath);

            }
            // 3 one child (left)
            else if (root->rightChild == nullNode) {

                str parentPath = root->parent;
                str leftChildPath = root->leftChild;

                AVLNode* leftChild = new AVLNode(directory);
                leftChild->readNode(leftChildPath);

                leftChild->parent = parentPath;
                leftChild->writeNode(leftChildPath);

                if (parentPath != nullNode) {
                    AVLNode* parent = new AVLNode(directory);
                    parent->readNode(parentPath);
                    if (parent->leftChild == path) {
                        root->leftChild = leftChildPath;
                    }
                    else {
                        parent->rightChild = leftChildPath;
                    }
                    parent->writeNode(parentPath);
                    delete parent;
                }
                fs::path filePath = directory / path.c_str();
                fs::remove(filePath);
                delete leftChild;
                path = leftChildPath;
                //readInNode(leftChildPath);
            }
            // 4 Two children
            else {
                // successor in-order?
                AVLNode* successor = new AVLNode(directory);
                str successorPath = root->rightChild;
                successor->readNode(successorPath);
                while (successor->leftChild != nullNode) {
                    successorPath = root->leftChild;
                    successor->readNode(successorPath);
                }

                Key successorKey = successor->key;
                // delete the successor from its subtree
                str right = deleteNode(root->rightChild, successorKey.value);
                root->rightChild = right;
                delete successor;

                root->key = successorKey;
                root->writeNode(path);
                str newPath = root->updateNodeFileName(path);

                //update parents child
                //str parentPath = root->parent;
                //if (parentPath != nullNode) {		//working ajeeb???
                //	AVLNode* parent = new AVLNode(directory);
                //	parent->readNode(parentPath);
                //	if (parent->leftChild == path) {
                //		parent->leftChild = newPath;
                //		parent->writeNode(parentPath);
                //	}
                //	else {
                //		parent->rightChild = newPath;
                //		parent->writeNode(parentPath);
                //	}
                //}
                path = newPath;

            }
            str tmp = path;
            if (path != nullNode) {
                tmp = root->checkRotation(path);
                root->updateHeight(path);
                root->writeNode(path);
            }
            delete root;
            return tmp;
        }

        str tmp = root->checkRotation(path);
        root->updateHeight(path);
        root->writeNode(path);
        delete root;
        return tmp;

    }

    void AVLTree::remove(str key) {
        this->rootPath = this->deleteNode(rootPath, key);
    }

    bool AVLTree::search(str key) {
        AVLNode* currNode = new AVLNode(directory);
        currNode->readNode(rootPath);
        while (currNode->key.value != key) {
            if (key < currNode->key.value) {
                if (currNode->leftChild == nullNode) {
                    delete currNode;
                    return false;
                }
                else {
                    str left = currNode->leftChild;
                    delete currNode;
                    currNode = new AVLNode(directory);
                    currNode->readNode(left);
                }
            }
            else if (key > currNode->key.value) {
                if (currNode->rightChild == nullNode) {
                    delete currNode;
                    return false;
                }
                else {
                    str right = currNode->rightChild;
                    delete currNode;
                    currNode = new AVLNode(directory);
                    currNode->readNode(right);
                }
            }
        }
        for (int i = 0; i < currNode->key.duplicates.size; i++)
            cout << currNode->key.duplicates[i].line << endl;
        delete currNode;
        return true;
    }

    bool AVLTree::search(str key, str column, int col) {
        AVLNode* currNode = new AVLNode(directory);
        currNode->readNode(rootPath);
        while (currNode->key.value != key) {
            if (key < currNode->key.value) {
                if (currNode->leftChild == nullNode) {
                    delete currNode;
                    return false;
                }
                else {
                    str left = currNode->leftChild;
                    delete currNode;
                    currNode = new AVLNode(directory);
                    currNode->readNode(left);
                }
            }
            else if (key > currNode->key.value) {
                if (currNode->rightChild == nullNode) {
                    delete currNode;
                    return false;
                }
                else {
                    str right = currNode->rightChild;
                    delete currNode;
                    currNode = new AVLNode(directory);
                    currNode->readNode(right);
                }
            }
        }
        for (int i = 0; i < currNode->key.duplicates.size; i++) {
            str line = currNode->key.duplicates[i].line;
            //get col value string from like , seperated
            str colValue = extractColumn(line.c_str(), col);
            if (colValue == column)
                cout << line << endl;

        }
        delete currNode;
        return true;
    }
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

    str AVLTree::getFilePath(str key) {
        AVLNode* currNode = new AVLNode(directory);
        str targ = rootPath;
        currNode->readNode(rootPath);
        while (currNode->key.value != key) {
            if (key < currNode->key.value) {
                if (currNode->leftChild == nullNode) {
                    delete currNode;
                    return "nullptr";
                }
                else {
                    str left = currNode->leftChild;
                    delete currNode;
                    currNode = new AVLNode(directory);
                    currNode->readNode(left);
                    targ = left;
                }
            }
            else if (key > currNode->key.value) {
                if (currNode->rightChild == nullNode) {
                    delete currNode;
                    return "nullptr";
                }
                else {
                    str right = currNode->rightChild;
                    delete currNode;
                    currNode = new AVLNode(directory);
                    currNode->readNode(right);
                    targ = right;
                }
            }
        }
        /*for (int i = 0; i < currNode->key.duplicates.size; i++)
            cout << currNode->key.duplicates[i].line << endl;*/
        delete currNode;
        return targ;
    }
