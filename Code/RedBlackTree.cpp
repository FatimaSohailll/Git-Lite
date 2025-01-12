#include "RedBlackTree.h"


RedBlackTree:: RedBlackTree(fs::path directory) :nullNode("nullNode.txt") {
        this->directory = directory;
        if (!fs::exists(directory))
            fs::create_directories(directory);
        //make null node
        fs::path filePath = directory / nullNode.c_str();
        fstream nullFile;
        nullFile.open(filePath);
        root = new RedBlackNode(directory); //null node
        root->color = 0; //null node is black
        treeRoot = nullNode;
        rootPath = nullNode;
        parentPath = nullNode;
        grandparentPath = nullNode;
        greatGrandparentPath = nullNode;
        writeNodeFile();
        nullFile.close();
        nodeCount = 0;
    }
    void RedBlackTree::updateRoot(RedBlackNode* newRoot, str newRootPath) { //only updates root path, not parent or gparent (like im doing that manually in insert etc)
        delete this->root;
        root = newRoot;
        rootPath = newRootPath;
        //grandparentPath = parentPath;
        //parentPath = root->parent;
    }
    int RedBlackTree::getColour(str nodePath) {//change to char* and use str->arr
        fs::path filePath = directory / nodePath.c_str();
        fstream nodeFile;
        nodeFile.open(filePath);
        char line[20] = { 0 };
        for (int i = 0; i < 2; i++)
            nodeFile.getline(line, 20);
        int colour = stoi(line);
        return colour;
    }
    int RedBlackTree::getParentColour() {
        return getColour(parentPath);
    }
    int RedBlackTree::getUncleAndColour(str& unclePath) {
        fstream nodeFile; //grandparent (father of uncle)
        nodeFile.open(directory / grandparentPath.c_str());
        char line[20] = { 0 };
        for (int i = 0; i < 3; i++)
            nodeFile.getline(line, 20);
        if (!strcmp(line, parentPath.c_str())) //uncle is right child
            nodeFile.getline(line, 20);
        unclePath = line; //set uncle path
        nodeFile.close();
        int color = getColour(unclePath);
        return color;
    }
    void RedBlackTree::restoreRoot() {
        readInNode(treeRoot);
        root->color = 0;
        writeNodeFile();
        parentPath = nullNode;
        grandparentPath = nullNode;
        greatGrandparentPath = nullNode;
    }
    void RedBlackTree::updateTreeRoot(str newRoot) {
        treeRoot = newRoot;
    }
    void RedBlackTree::readInNode(str nodeFilePath) { //file to node
        /*
            key
            color
            left child
            right child
            parent path
            path of block
            line number
        */
        RedBlackNode* tmp = new RedBlackNode(this->directory);
        const int LINE_LENGTH = 25;
        fstream nodeFile;
        nodeFile.open(directory / nodeFilePath.c_str());
        char line[LINE_LENGTH] = { 0 };
        if (!nodeFile.is_open())
            return;

        nodeFile.getline(line, LINE_LENGTH); //key
        tmp->key = stoi(line);
        nodeFile.getline(line, LINE_LENGTH); //color
        tmp->color = stoi(line);
        nodeFile.getline(line, LINE_LENGTH); //left child
        tmp->leftChild = line;
        nodeFile.getline(line, LINE_LENGTH); //right child
        tmp->rightChild = line;
        nodeFile.getline(line, LINE_LENGTH); //parent
        tmp->parent = line;
        nodeFile.getline(line, LINE_LENGTH); //record
        tmp->record = line;
        updateRoot(tmp, nodeFilePath);
        nodeFile.close();
    }
    void RedBlackTree::writeNodeFile() { //node to file
        ofstream nodeFile;
        nodeFile.open(directory / rootPath.c_str());
        nodeFile << to_string(root->key) << "\n"; //key
        nodeFile << to_string(root->color) << "\n"; //color
        nodeFile << root->leftChild << "\n"; //left child 
        nodeFile << root->rightChild << "\n"; //right child 
        nodeFile << root->parent << "\n"; //parent
        nodeFile << root->record << "\n"; //record
        nodeFile.close();
    }
    //helpers for reorient CAN GET RID OF THESE IF YOU USE < >
    str RedBlackTree::getLeftChild(str nodePath) {
        fstream nodeFile;
        nodeFile.open(directory / parentPath.c_str());
        char line[20] = { 0 };
        for (int i = 0; i < 3; i++)
            nodeFile.getline(line, 20);
        str leftChild = line;
        return leftChild;
    }
    str RedBlackTree::getParentLeft() {
        return getLeftChild(parentPath);
    }
    str RedBlackTree::getGrandparentLeft() {
        return getLeftChild(grandparentPath);
    }
    str RedBlackTree::getGreatGrandparentLeft() {
        return getLeftChild(greatGrandparentPath);
    }
    //recolouring
    void RedBlackTree::singleRotRecolour() { //swap parent and granparent colour (parent red, gparent black)
        readInNode(parentPath);
        root->color = 0;
        writeNodeFile();
        readInNode(grandparentPath);
        root->color = 1;
        writeNodeFile();
        readInNode(rootPath);
    }
    void RedBlackTree::doubleRotRecolour() {
        root->color = 0;
        writeNodeFile();
        readInNode(grandparentPath);
        root->color = 1;
        writeNodeFile();
        readInNode(rootPath);
    }
    //get keys (instead of ^^^)
    int RedBlackTree::getKey(str nodePath) {
        fstream nodeFile;
        nodeFile.open(directory / nodePath.c_str());
        char line[20] = { 0 };
        nodeFile.getline(line, 20);
        int key = stoi(line);
        return key;
    }
    void RedBlackTree::reOrient() {

        int parentColor = getParentColour();
        if (parentColor == 0) //black parent means no red red conflict
            return;
        //red-red conflict
        while (parentColor == 1) {
            str unclePath = "";
            str t = rootPath;
            parentPath = root->parent;
            readInNode(root->parent);
            grandparentPath = root->parent;
            readInNode(t);
            int uncleColour = getUncleAndColour(unclePath);
            if (uncleColour == 1) { //red uncle
                //colour parent black
                readInNode(parentPath);
                root->color = 0;
                writeNodeFile();
                //colour uncle black
                readInNode(unclePath);
                root->color = 0;
                writeNodeFile();
                //colour grandparent red
                readInNode(root->parent);
                root->color = 1;
                writeNodeFile();
            }
            else { //black uncle
                int newKey = root->key;
                str currentNode = rootPath;
                str k = currentNode;
                parentPath = root->parent;
                readInNode(parentPath);
                grandparentPath = root->parent;
                readInNode(k);

                if (getKey(parentPath) < getKey(grandparentPath)) { //right rotate as parent is a left child
                    if (newKey > getKey(parentPath)) { //if right of parent, double rotate
                        doubleRotRecolour();
                        readInNode(parentPath);
                        k = doubleRightRotation(grandparentPath);
                    }
                    else {
                        singleRotRecolour();
                        readInNode(grandparentPath);
                        k = singleRightRotate();
                    }
                    readInNode(currentNode);
                }
                else { //left rotate
                    if (newKey < getKey(parentPath)) {  //if left of parent, double rotate
                        doubleRotRecolour();
                        readInNode(parentPath);
                        k = doubleLeftRotation(grandparentPath);
                    }
                    else {
                        singleRotRecolour();
                        readInNode(grandparentPath);
                        k = singleLeftRotate();
                    }
                    readInNode(currentNode);
                }

                str temp = rootPath;

                readInNode(k);
                if (root->parent != nullNode) { //k is not new root of whole tree
                    readInNode(root->parent);
                    if (newKey < root->key)
                        root->leftChild = k;
                    else
                        root->rightChild = k;
                    writeNodeFile();
                    readInNode(temp);
                }
                else { //k is new root of whole tree
                    updateTreeRoot(k);
                }
                readInNode(currentNode);
                return; //no percolation up
            }
            //get parent colour (to percolate up in case of red uncle)
            //readInNode(rootPath);
            str tmp = rootPath;
            readInNode(root->parent);
            parentColor = root->color;
            readInNode(tmp);


            //parentColor = getParentColour(); //parent of grandparent as curr root is initial grandparent(red)
        }
    }
    void RedBlackTree::insert(int key, str record) {
        while (rootPath != nullNode) { //find place to insert
            if (key < root->key) {
                greatGrandparentPath = grandparentPath;
                grandparentPath = parentPath;
                parentPath = rootPath;
                readInNode(root->leftChild);
            }
            else {
                greatGrandparentPath = grandparentPath;
                grandparentPath = parentPath;
                parentPath = rootPath;
                readInNode(root->rightChild);
            }
        }
        //insert
        RedBlackNode* newNode = new RedBlackNode(key, parentPath, this->directory, record);
        if (parentPath == nullNode) //root of tree is black
            newNode->color = 0;
        char* filename = generateFileName(nodeCount++);
        updateRoot(newNode, filename);
        writeNodeFile();

        if (parentPath != nullNode) { //make it parents child
            str childPath = rootPath;
            readInNode(parentPath);
            if (key < root->key)
                root->leftChild = childPath;
            else
                root->rightChild = childPath;
            writeNodeFile();
            readInNode(filename); //read new one back
        }
        else { //root of whole tree 
            updateTreeRoot(filename);
            return;
        }
        if (grandparentPath == nullNode) { //no grandparent means no conflict (root is black this is red)
            restoreRoot(); //restore root
            return;
        }
        reOrient();
        restoreRoot(); //restore root for next time
    }



    //rotations (from avl)
    str RedBlackTree::doubleLeftRotation(str& rootP) {  //the parameter is a bit confusing...i think we need to generalise these a bit more
        str newRoot = singleRightRotate();
        readInNode(rootP);
        root->rightChild = newRoot;
        newRoot = singleLeftRotate();
        readInNode(newRoot);
        return newRoot;
    }
    str RedBlackTree::doubleRightRotation(str& rootP) {
        str newRoot = singleLeftRotate();
        //readInNode(newRoot);
        readInNode(rootP);
        root->leftChild = newRoot;
        newRoot = singleRightRotate();
        readInNode(newRoot);
        return newRoot;
    }
    str RedBlackTree::singleLeftRotate() {
        str thisNode = rootPath;
        str myParent = root->parent;
        str k = root->rightChild; //candidate
        readInNode(k);
        str kLeft = root->leftChild; //get ks right
        root->leftChild = root->parent; //ks right is this one, its parent
        root->parent = myParent; //ks parent is this ones parent
        writeNodeFile();
        readInNode(thisNode);
        root->rightChild = kLeft;
        root->parent = k;
        writeNodeFile();
        readInNode(kLeft);
        if (root) {
            root->parent = thisNode;
            writeNodeFile();
        }
        return k;
    }

    str RedBlackTree::singleRightRotate() {
        str thisNode = rootPath;
        str myParent = root->parent;
        str k = root->leftChild; //candidate
        readInNode(k);
        str kRight = root->rightChild; //get ks right
        root->rightChild = root->parent; //ks right is this one, its parent
        root->parent = myParent; //ks parent is this ones parent
        writeNodeFile();
        readInNode(thisNode);
        root->leftChild = kRight;
        root->parent = k;
        writeNodeFile();
        readInNode(kRight);
        if (root) {
            root->parent = thisNode;
            writeNodeFile();
        }
        return k;
    }


    str RedBlackTree::search(int val) {
        readInNode(treeRoot);
        str temp = rootPath;
        while (rootPath != nullNode) {
            if (root->key > val) {
                readInNode(root->leftChild);
            }
            else if (root->key == val) {
                str ans = rootPath;
                readInNode(temp);
                return ans;
            }
            else {
                readInNode(root->rightChild);
            }
        }

        return nullNode;
    }
    str RedBlackTree::getSuccessor(str& before) {
        str temp = rootPath;
        //2 children
        readInNode(before);
        if (root->leftChild != nullNode && root->rightChild != nullNode) {
            readInNode(root->leftChild);
            str temp2 = rootPath;
            while (root->rightChild != nullNode) {
                readInNode(root->rightChild);
            }
            str returning = rootPath;
            readInNode(temp);
            return returning;
        }

        //leaf
        if (root->leftChild == nullNode && root->rightChild == nullNode) {
            readInNode(temp);
            return nullNode;
        }

        //one bacha :)
        if (root->leftChild != nullNode) {
            str ret = root->leftChild;
            readInNode(temp);
            return ret;
        }
        else {
            str ret = root->rightChild;
            readInNode(temp);
            return ret;
        }
    }

    void RedBlackTree::fixDoubleBlack(str x) {

        if (x == treeRoot) {
            return;
        }
        readInNode(x);
        str parent = root->parent;
        readInNode(parent);
        RedBlackNode p = *root;
        str sibling = (p.leftChild == x) ? p.rightChild : p.leftChild;
        readInNode(sibling);
        RedBlackNode s = *root;

        if (sibling == nullNode) {
            fixDoubleBlack(parent);
        }
        else {
            if (s.color == 1) {
                p.color = 1;
                s.color = 0;
                readInNode(sibling);
                root->color = s.color;
                writeNodeFile();
                readInNode(parent);
                root->color = p.color;
                str k = rootPath;
                str prevChild = x;
                writeNodeFile();		///assign parent?
                if (sibling == p.leftChild) {
                    k = singleRightRotate();
                }
                else {
                    k = singleLeftRotate();
                }
                fixDoubleBlack(x);
                str tempp = rootPath;
                readInNode(k);
                if (root->parent == nullNode) {
                    updateTreeRoot(k);
                }
                else {
                    readInNode(root->parent);
                    if (root->leftChild == prevChild) {
                        root->leftChild = k;
                    }
                    else {
                        root->rightChild = k;
                    }
                    writeNodeFile();
                }
                readInNode(tempp);

            }
            else {
                bool siblinghasRedChild = false;
                readInNode(s.rightChild);
                if (root->color == 1) {
                    siblinghasRedChild = true;
                }
                readInNode(s.leftChild);
                if (root->color == 1) {
                    siblinghasRedChild = true;
                }
                //readInNode(x);
                if (siblinghasRedChild) {
                    if (s.leftChild != nullNode && root->color == 1) { //left red
                        if (sibling == p.leftChild) {
                            //left left
                            root->color = s.color;
                            s.color = p.color;
                            writeNodeFile();
                            readInNode(sibling);
                            root->color = s.color;
                            writeNodeFile();
                            readInNode(parent);
                            str prevChild = rootPath;
                            str k = singleRightRotate();
                            readInNode(k);
                            if (root->parent == nullNode) {
                                updateTreeRoot(k);
                            }
                            else {
                                readInNode(root->parent);
                                if (root->leftChild == prevChild) {
                                    root->leftChild = k;
                                }
                                else {
                                    root->rightChild = k;
                                }
                                writeNodeFile();
                            }
                        }
                        else { // right left
                            root->color = p.color;
                            writeNodeFile();
                            readInNode(sibling);	//needs fixing maybe
                            str prevChild1 = rootPath;
                            str k1 = singleRightRotate();
                            readInNode(k1);
                            if (root->parent == nullNode) {
                                updateTreeRoot(k1);
                            }
                            else {
                                readInNode(root->parent);
                                if (root->leftChild == prevChild1) {
                                    root->leftChild = k1;
                                }
                                else {
                                    root->rightChild = k1;
                                }
                                writeNodeFile();
                            }
                            writeNodeFile();
                            readInNode(parent);
                            str prevChild2 = rootPath;
                            str k2 = singleLeftRotate();
                            readInNode(k2);
                            if (root->parent == nullNode) {
                                updateTreeRoot(k2);
                            }
                            else {
                                readInNode(root->parent);
                                if (root->leftChild == prevChild2) {
                                    root->leftChild = k2;
                                }
                                else {
                                    root->rightChild = k2;
                                }
                                writeNodeFile();
                            }
                            writeNodeFile();
                        }
                    }
                    else {
                        if (sibling == p.leftChild) {
                            //left right
                            readInNode(s.rightChild);
                            root->color = p.color;
                            writeNodeFile();
                            readInNode(sibling);
                            str prevChild1 = rootPath;
                            str k1 = singleLeftRotate();			//needs fixing maybe
                            readInNode(k1);
                            if (root->parent == nullNode) {
                                updateTreeRoot(k1);
                            }
                            else {
                                readInNode(root->parent);
                                if (root->leftChild == prevChild1) {
                                    root->leftChild = k1;
                                }
                                else {
                                    root->rightChild = k1;
                                }
                                writeNodeFile();
                            }
                            writeNodeFile();
                            readInNode(parent);
                            str prevChild2 = rootPath;
                            str k2 = singleRightRotate();
                            readInNode(k2);
                            if (root->parent == nullNode) {
                                updateTreeRoot(k2);
                            }
                            else {
                                readInNode(root->parent);
                                if (root->leftChild == prevChild2) {
                                    root->leftChild = k2;
                                }
                                else {
                                    root->rightChild = k2;
                                }
                                writeNodeFile();
                            }
                        }
                        else {
                            //right right
                            readInNode(s.rightChild);
                            root->color = s.color;
                            writeNodeFile();
                            s.color = p.color;
                            readInNode(sibling);
                            root->color = s.color;
                            writeNodeFile();
                            readInNode(parent);
                            str prevChild = x;
                            str k = singleLeftRotate();
                            readInNode(k);
                            if (root->parent == nullNode) {
                                updateTreeRoot(k);
                            }
                            else {
                                readInNode(root->parent);
                                if (root->leftChild == prevChild) {
                                    root->leftChild = k;
                                }
                                else {
                                    root->rightChild = k;
                                }
                                writeNodeFile();
                            }
                        }
                    }
                    readInNode(parent);
                    root->color = 0;
                    writeNodeFile();
                }
                else { //2 black children
                    readInNode(sibling);
                    root->color = 1;
                    writeNodeFile();
                    if (p.color == 0) {
                        fixDoubleBlack(parent);
                    }
                    else {
                        readInNode(parent);
                        root->color = 0;
                        writeNodeFile();
                    }
                }

            }
        }
    }


    void RedBlackTree::removeNode(str& toDelete) {
        readInNode(toDelete);
        str u = getSuccessor(toDelete);
        readInNode(u);
        RedBlackNode succ = *root;
        readInNode(toDelete);
        //u and v are both black (successor and toDelete)
        bool uvBlack = ((u == nullNode || succ.color == 0) && (root->color == 0));
        str parent = root->parent;
        readInNode(parent);
        RedBlackNode p = *root;
        readInNode(toDelete);

        if (u == nullNode) {

            //if us is null that means v is leafNOde
            if (rootPath == treeRoot) {
                updateRoot(nullptr, nullNode);
            }
            else {
                if (uvBlack) {
                    //u and v both black
                    /////v is leaf, fix double black at v
                    fixDoubleBlack(rootPath);
                }
                else {
                    //u or v is red
                    if (p.leftChild == rootPath) {
                        readInNode(p.rightChild);
                    }
                    else {
                        readInNode(p.leftChild);
                    }
                    ///notNUll sibling make it red
                    if (rootPath != nullNode) {
                        root->color = 1;
                        writeNodeFile();
                    }
                    readInNode(toDelete);

                }
                //delete v from the tree
                if (p.leftChild == toDelete) {
                    p.leftChild = nullNode;
                    root->leftChild = nullNode;
                    writeNodeFile();
                }
                else {
                    p.rightChild = nullNode;
                    root->rightChild = nullNode;
                    writeNodeFile();
                }

            }
            /*readInNode(u);
            root->blockOffset = succ.blockOffset;
            root->blockPtr = succ.blockPtr;
            root->color = succ.color;
            root->key = succ.key;
            root->leftChild = succ.leftChild;
            root->rightChild = succ.rightChild;
            root->parent = succ.parent;
            writeNodeFile();
            readInNode(parent);
            root->blockOffset = p.blockOffset;
            root->blockPtr = p.blockPtr;
            root->color = p.color;
            root->key = p.key;
            root->leftChild = p.leftChild;
            root->rightChild = p.rightChild;
            root->parent = p.parent;
            writeNodeFile();*/
            readInNode(toDelete);
            readInNode(root->parent);
            if (root->leftChild == toDelete) {
                root->leftChild = nullNode;
            }
            else if (root->rightChild == toDelete) {
                root->rightChild = nullNode;
            }
            writeNodeFile();
            filesystem::remove(directory / toDelete.c_str());
            return;
        }
        //only one bacha of toDelete :(
        if (root->leftChild == nullNode || root->rightChild == nullNode) {
            if (rootPath == treeRoot) {
                // updateRoot(&succ, u);
                root->key = succ.key;
                root->color = succ.color;
                root->leftChild = nullNode;
                root->rightChild = nullNode;
                writeNodeFile();
                filesystem::remove(directory / u.c_str());
                //updateNodeFileName();
                treeRoot = rootPath;
            }
            else {
                //detach toDelete from tree and move u up
                str t = rootPath;
                if (p.leftChild == toDelete) {
                    readInNode(root->parent);
                    root->leftChild = u;
                    writeNodeFile();
                }
                else {
                    readInNode(root->parent);
                    root->rightChild = u;
                    writeNodeFile();
                }
                filesystem::remove(directory / toDelete.c_str());
                readInNode(u);
                root->parent = parent;
                writeNodeFile();
                if (uvBlack) {
                    //u and v both black, fix double black at u
                    fixDoubleBlack(rootPath);
                }
                else {
                    //u or v red, color u black
                    root->color = 0;
                    writeNodeFile();
                }
            }
            return;
        }
        //two children
        //swap values with successor and recurse
        //swapNodes(toDelete, u);
        readInNode(toDelete);
        int key = root->key;
        str rec = root->record;
        root->key = succ.key;
        root->record = succ.record;
        writeNodeFile();
        readInNode(u);
        root->key = key;
        root->record = rec;
        writeNodeFile();
        removeNode(u);
    }

    void RedBlackTree::deleteKey(int val) {
        if (treeRoot == nullNode) {
            return;
        }

        str removepath = search(val);	//get path of key

        if (removepath == nullNode) {
            return;		//not found
        }

        removeNode(removepath);
        readInNode(treeRoot);

    }

    void RedBlackTree::inOrderTraversal(str path) {
        if (path == nullNode) {
            return;
        }
        readInNode(path);
        inOrderTraversal(root->leftChild);
        readInNode(path);
        cout << root->key << " ";
        inOrderTraversal(root->rightChild);
    }
