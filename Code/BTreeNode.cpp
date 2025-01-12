#include "BTreeNode.h"

BTreeNode::BTreeNode(fs::path dir, int m) :m(m), numKey(0), parent("nullNode.txt"), nullNode("nullNode.txt"), isLeaf(true) { //m-way
        this->directory = dir;
        keys = new Key[m];
        child = new str[m + 1];
        for (int i = 0; i <= m; i++)
            child[i] = "nullNode.txt";
    }
    void  BTreeNode::writeNode(str nodePath) {
        fs::path filePath = directory / nodePath.c_str();
        ofstream nodeFile;
        nodeFile.open(filePath);
        //keys
        nodeFile << this->numKey << "\n";
        for (int i = 0; i < this->numKey; i++) {
            nodeFile << this->keys[i].value << "\n";
            int numInstances = this->keys[i].duplicates.size;
            nodeFile << numInstances << '\n';
            for (int j = 0; j < numInstances; j++) {
                nodeFile << this->keys[i].duplicates[j].line << '\n';
                //nodeFile << "[record]" << '\n';
            }
        }
        //children
        if (this->isLeaf)
            nodeFile << '0' << '\n';
        else {
            int numChild = this->numKey + 1;
            nodeFile << numChild << '\n';
            for (int i = 0; i < numChild; i++) {
                nodeFile << this->child[i] << '\n';
            }
        }
        nodeFile << this->parent << '\n';
        nodeFile.close();
    }
    void  BTreeNode::readNode(str nodePath) {
        fs::path filePath = directory / nodePath.c_str();
        ifstream nodeFile;
        nodeFile.open(filePath);
        const int LINE_LEN = 255;
        char line[LINE_LEN] = { 0 };
        //keys
        nodeFile.getline(line, LINE_LEN);
        int numKey = stoi(line);
        this->numKey = numKey;
        for (int i = 0; i < numKey; i++) {
            nodeFile.getline(line, LINE_LEN);
            this->keys[i].value = line; //key
            nodeFile.getline(line, LINE_LEN);
            int numInstances = stoi(line);
            for (int j = 0; j < numInstances; j++) {
                nodeFile.getline(line, LINE_LEN);
                this->keys[i].insertDuplicates(line);
            }
        }
        //children
        nodeFile.getline(line, LINE_LEN);
        int numChild = stoi(line);
        if (numChild > 0)
            this->isLeaf = false;
        for (int i = 0; i < numChild; i++) {
            nodeFile.getline(line, LINE_LEN);
            this->child[i] = line;
        }
        nodeFile.getline(line, LINE_LEN);
        this->parent = line;
        nodeFile.close();
    }
    bool  BTreeNode::isFull() {
        return (numKey == m);
    }
    bool  BTreeNode::isDefecit() {
        return (numKey < ceil(m / 2.0) - 1);
    }
    //display (for testing)
    void  BTreeNode::display() {
        cout << "keys: ";
        for (int i = 0; i < numKey; i++)
            cout << keys[i] << " ";
        cout << "  ,children: ";
        for (int i = 0; i < numKey + 1; i++)
            cout << child[i] << " ";
    }
    //search
    int  BTreeNode::search(str searchKey) { //index of searchKey
        for (int i = 0; i < m - 1; i++) {
            if (keys[i].value == searchKey)
                return i;
        }
        return -1;
    }
    //insert
    int  BTreeNode::insert(str newKey, str record) {
        int index = 0;
        while (keys[index].value < newKey&& keys[index].value != "-1")
            index++;
        if (keys[index].value == newKey) { //duplicate
            keys[index].insertDuplicates(record);
            return index;
        }
        //index is the index to insert
        for (int j = m - 1; j > index; j--) { //shift keys greater than
            keys[j] = keys[j - 1];
        }
        for (int j = m; j > index + 1; j--) { //shift children (right children)
            child[j] = child[j - 1];
        }
        keys[index].clearKey();
        keys[index].value = newKey;
        keys[index].insertDuplicates(record);
        this->numKey++;
        return index; //return inseted index
    }
    int  BTreeNode::insert(Key newKey) { //always a unique key
        int index = 0;
        while (keys[index].value < newKey.value&& keys[index].value != "-1")
            index++;
        //index is the index to insert
        for (int j = m - 1; j > index; j--) { //shift keys greater than
            keys[j] = keys[j - 1];
        }
        for (int j = m; j > index + 1; j--) { //shift children (right children)
            child[j] = child[j - 1];
        }
        keys[index] = newKey;
        //keys[index].addressList.insert(Address(blockPtr, blockOffset));
        this->numKey++;
        return index; //return inseted index
    }
    void  BTreeNode::remove(int k, bool removeAll) { //remove kth key
        if (this->keys[k].duplicates.size > 1 && !removeAll) {
            this->keys[k].duplicates.remove(); //remove first instance in sll
            return;
        }
        this->keys[k].duplicates.clearList();
        for (int i = k + 1; i < m; i++) {
            keys[i - 1] = keys[i];
        }
        this->numKey--;
    }
    void  BTreeNode::clearNode() {
        delete[] this->keys;
        delete[] this->child;
        keys = new Key[m];
        child = new str[m + 1];
        for (int i = 0; i <= m; i++)
            child[i] = "nullNode.txt";
        this->numKey = 0;
        this->parent = nullNode;
    }
    Key  BTreeNode::getSuccessor(int i) {
        //i is the key in the node whose sucessor i want
        BTreeNode* successor = new BTreeNode(directory, m);
        successor->readNode(this->child[i + 1]);
        while (successor->child[0] != nullNode) {
            str next = successor->child[0];
            delete successor;
            successor = new BTreeNode(directory, m);
            successor->readNode(next);
        }
        Key successorKey = successor->keys[0];
        delete successor;
        return successorKey;
    }
    Key  BTreeNode::getPredecessor(int i) {
        //i is the key in the node whose predecessor i want
        BTreeNode* predecessor = new BTreeNode(directory, m);
        predecessor->readNode(this->child[i]);
        while (predecessor->child[predecessor->numKey] != nullNode) {
            str next = predecessor->child[predecessor->numKey];
            delete predecessor;
            predecessor = new BTreeNode(directory, m);
            predecessor->readNode(next);
        }
        Key predecessorKey = predecessor->keys[predecessor->numKey - 1];
        delete predecessor;
        return predecessorKey;
    }
    //split cth child of root, if its full
    void  BTreeNode::splitChild(str path, BTreeNode* borrowChild, int c, int& numNodes) { //just for now until we change naming convetnion

        int medianIndex = borrowChild->numKey / 2;
        Key medianKey = borrowChild->keys[medianIndex]; //median tp push up into parent
        borrowChild->keys[medianIndex].clearKey(); //remove median from split child

        //remove median keys and right children onwards from split child
        str rightChildPath = generateFileName(numNodes);
        BTreeNode* rightSibling = new BTreeNode(directory, m);
        rightSibling->isLeaf = borrowChild->isLeaf;

        int j = 0;
        for (int i = medianIndex + 1; i < borrowChild->numKey; i++) { //shift keys to right sibling
            rightSibling->keys[j] = borrowChild->keys[i];
            rightSibling->numKey++;
            borrowChild->keys[i].clearKey();
            j++;
        }
        j = 0;
        for (int i = medianIndex + 1; i <= borrowChild->numKey; i++) { //shift to right sibling
            rightSibling->child[j] = borrowChild->child[i];
            if (rightSibling->child[j] != nullNode) {
                BTreeNode* tmp = new BTreeNode(directory, m);
                tmp->readNode(rightSibling->child[j]);
                tmp->parent = rightChildPath;
                tmp->writeNode(rightSibling->child[j]);
            }
            borrowChild->child[i] = nullNode;
            j++;
        }
        borrowChild->numKey /= 2; //halve num keys

        borrowChild->parent = path;
        rightSibling->parent = path; //same parent as sibling
        numNodes++; //the right child adds 1

        //place median key in parent and adjust new child (right sibling)
        for (int j = m - 1; j > c; j--) //shift greater keys..
            this->keys[j] = this->keys[j - 1];
        for (int j = m; j > c + 1; j--) //..and their right children to make space for median key
            this->child[j] = this->child[j - 1];
        this->keys[c] = medianKey; //median in parent
        this->child[c + 1] = rightChildPath; //assign right sibling as right child of median
        this->numKey++;

        //update files
        this->writeNode(path);
        borrowChild->writeNode(this->child[c]);
        rightSibling->writeNode(rightChildPath);

        delete rightSibling;
    }
    void  BTreeNode::borrowFromLeft(str path, int c) { //child[c] needs a borrow

        BTreeNode* borrowChild = new BTreeNode(directory, m);
        borrowChild->readNode(this->child[c]); //left

        BTreeNode* sibling = new BTreeNode(directory, m);
        sibling->readNode(this->child[c - 1]);
        Key parentKeyDown = this->keys[c - 1];

        borrowChild->insert(parentKeyDown);
        borrowChild->child[1] = borrowChild->child[0]; //move leftest child one ahead for siblings rightmost child

        Key siblingKeyUp = sibling->keys[sibling->numKey - 1]; //rightest key moves up into parent
        BTreeNode* siblingRightChild = new BTreeNode(directory, m);
        siblingRightChild->readNode(sibling->child[sibling->numKey]); //rightmost child becomes leftmost child of borrower

        this->keys[c - 1] = siblingKeyUp; //parent gets right child (lenders) rightmost key

        borrowChild->child[0] = sibling->child[sibling->numKey]; //borrower gets lenders rightmost child
        siblingRightChild->parent = this->child[c]; //set rightmost child parent as borrower
        siblingRightChild->writeNode(sibling->child[sibling->numKey]);

        sibling->keys[sibling->numKey - 1].clearKey();
        sibling->child[sibling->numKey] = nullNode;
        sibling->numKey--;

        if (borrowChild->child[0] != nullNode)
            borrowChild->isLeaf = false;

        borrowChild->writeNode(this->child[c]);
        sibling->writeNode(this->child[c - 1]);
        this->writeNode(path);

        delete borrowChild;
        delete sibling;
        delete siblingRightChild;

    }
    void  BTreeNode::borrowFromRight(str path, int c) {//child[c] is thr borrower

        BTreeNode* borrowChild = new BTreeNode(directory, m);
        borrowChild->readNode(this->child[c]);

        BTreeNode* sibling = new BTreeNode(directory, m);
        sibling->readNode(this->child[c + 1]);

        Key parentKeyDown = this->keys[c];

        borrowChild->insert(parentKeyDown); //parents key is rightmost key of borrower

        Key siblingKeyUp = sibling->keys[0];
        BTreeNode* siblingLeftChild = new BTreeNode(directory, m);
        siblingLeftChild->readNode(sibling->child[0]);
        siblingLeftChild->parent = this->child[c]; //set leftmost child parent as borrower
        siblingLeftChild->writeNode(sibling->child[0]);

        borrowChild->child[borrowChild->numKey] = sibling->child[0]; //borrower gets lenders leftmost child
        if (borrowChild->child[0] != nullNode)
            borrowChild->isLeaf = false;

        for (int i = 1; i < sibling->numKey; i++) //shift siblings keys back
            sibling->keys[i - 1] = sibling->keys[i];
        for (int i = 1; i <= sibling->numKey; i++) //and children back to 'remove' the lent key and child
            sibling->child[i - 1] = sibling->child[i];
        sibling->numKey--;

        this->keys[c] = siblingKeyUp; //parent gets left child (lenders) leftmost key

        borrowChild->writeNode(this->child[c]);
        sibling->writeNode(this->child[c + 1]);
        this->writeNode(path);

        delete borrowChild;
        delete sibling;
        delete siblingLeftChild;
    }
    void  BTreeNode::merge(str path, int c) { //merge child[c] with child[c+1]

        BTreeNode* borrowChild = new BTreeNode(directory, m);
        borrowChild->readNode(this->child[c]);
        BTreeNode* sibling = new BTreeNode(directory, m);
        sibling->readNode(this->child[c + 1]);
        str siblingPath = this->child[c + 1];
        Key parentKeyDown = this->keys[c];

        borrowChild->keys[borrowChild->numKey] = parentKeyDown;
        borrowChild->numKey++;

        for (int i = 0; i < sibling->numKey; i++) { //merge keys and children
            borrowChild->keys[borrowChild->numKey] = sibling->keys[i];
            borrowChild->child[borrowChild->numKey] = sibling->child[i];
            if (sibling->child[i] != nullNode) {
                BTreeNode* tmp = new BTreeNode(directory, m);
                tmp->readNode(sibling->child[i]);
                tmp->parent = this->child[c];
                tmp->writeNode(sibling->child[i]);
                delete tmp;
            }
            borrowChild->numKey++;
        }
        borrowChild->child[borrowChild->numKey] = sibling->child[sibling->numKey];
        if (sibling->child[sibling->numKey] != nullNode) {
            BTreeNode* tmp = new BTreeNode(directory, m);
            tmp->readNode(sibling->child[sibling->numKey]);
            tmp->parent = this->child[c];
            tmp->writeNode(sibling->child[sibling->numKey]);
            delete tmp;
        }

        //borrowChild->numKey += sibling->numKey; //sbiling plus 1 from parent

        for (int j = c + 1; j < this->numKey; j++) //remove key that was moved down from parent
            this->keys[j - 1] = this->keys[j];
        for (int j = c + 2; j <= this->numKey; j++)
            this->child[j - 1] = this->child[j];
        this->numKey--;

        fs::path filePath = directory / siblingPath.c_str();
        fs::remove(filePath); //delete sibling node

        borrowChild->writeNode(this->child[c]);
        this->writeNode(path);

        delete borrowChild;
        delete sibling;

    }
    void  BTreeNode::fill(str path, int c) { //make up defecit by borrowing/merging
        if (c > 0) {
            BTreeNode* lChild = new BTreeNode(directory, m);
            lChild->readNode(this->child[c - 1]);
            if (lChild->numKey > ceil(m / 2.0) - 1) { //left sibling has extra
                this->borrowFromLeft(path, c);
                delete lChild;
                return;
            }
        }
        if (c < 0) {
            BTreeNode* rChild = new BTreeNode(directory, m);
            rChild->readNode(this->child[c + 1]);
            if (rChild->numKey > ceil(m / 2.0) - 1) {
                this->borrowFromRight(path, c);
                delete rChild;
                return;
            }
        }
        //couldnt borrow,merge
        if (c < this->numKey)
            this->merge(path, c); //merge right sibling (has right)
        else
            this->merge(path, c - 1); //merge left with this as right
    }
    void  BTreeNode::removeFromLeaf(str path, int i, bool removeAll) {
        this->remove(i, removeAll);
        this->writeNode(path);
    }
    void  BTreeNode::removeFromInternal(str path, int i) {

        Key delKey = this->keys[i];

        BTreeNode* tchild = new BTreeNode(directory, m);
        //tchild->readNode(this->child[i]);//predecessor brnach
        //if (tchild->numKey >= ceil(m/2)) {
        //	Key predecessor = this->getPredecessor(i);
        //	this->keys[i] = predecessor; //replace with predecessor
        //	//remove predecessor from subtree
        //	tchild->removeNode(path,predecessor.value);
        //	this->writeNode(path);
        //	delete tchild;
        //	return;
        //}

        //delete tchild;
        //tchild = new BTreeNode(m);
        tchild->readNode(this->child[i + 1]); //sucessor branch
        //if (tchild->numKey > ceil(m/2)) {
        Key successor = this->getSuccessor(i);
        this->keys[i] = successor;  //replace with successor
        //remove sucessor from subtree
        tchild->removeNode(this->child[i + 1], successor.value, true);
        if (tchild->isDefecit()) {
            this->fill(path, i + 1);
        }
        this->writeNode(path);
        delete tchild;
        return;
        //}
        //else merge 
        //this->merge(path,i); //brings it down into left child
        //this->remove(i); //delete from node

        this->writeNode(path);
    }
    BTreeNode* BTreeNode::removeNode(str path, str key, bool removeAll) { //removeAll tells whether to remove all duplicates or just one
        int index = this->search(key);
        if (index != -1) { //present in node
            if (this->isLeaf)
                this->removeFromLeaf(path, index, removeAll);
            else {
                this->removeFromInternal(path, index);
                //check if defecit???
            }
            return this;
        }
        else { //traverse into correct child
            if (this->isLeaf)
                return this; //not in tree
            int c = 0;
            while (this->keys[c].value < key && c < this->numKey)
                c++;
            BTreeNode* tchild = new BTreeNode(directory, m);
            tchild->readNode(this->child[c]);
            tchild->removeNode(this->child[c], key, removeAll);
            //check if child has defecit
            if (tchild->isDefecit()) {
                this->fill(path, c);
            }
            delete tchild;
            return this;
        }
    }
    BTreeNode:: ~BTreeNode() {
        if (keys)
            delete[] keys;
        if (child)
            delete[] child;
    }
