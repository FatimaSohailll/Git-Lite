#include "GitCommands.h"

void compareNodesAVL(str merkel1, str merkel2, fs::path main, fs::path branch) {

    if (merkel1 == "nullNode.txt" || merkel2 == "nullNode.txt")
        return;

    merkelNode* node1 = new merkelNode;
    node1->readNode(merkel1, main / fs::path("merkle"));
    merkelNode* node2 = new merkelNode;
    node2->readNode(merkel2, branch / fs::path("merkle"));

    if (node1->Hash == node2->Hash)
        return;

    str leftmerkel1 = node1->Path1;
    str rightmerkel1 = node1->Path2;
    str leftmerkel2 = node2->Path1;
    str rightmerkel2 = node2->Path2;
    compareNodesAVL(leftmerkel1, leftmerkel2, main, branch);
    compareNodesAVL(rightmerkel1, rightmerkel2, main, branch);

    if (node1->isLeaf() && node2->isLeaf()) {

        AVLNode* treenode1 = new AVLNode(main);
        AVLNode* treenode2 = new AVLNode(branch);

        //get node paths
        char t1[100];
        char t2[100];
        fstream file1;
        file1.open(main / fs::path("merkle") / merkel1.c_str());
        fstream file2;
        file2.open(branch / fs::path("merkle") / merkel1.c_str());
        for (int i = 0; i < 4; i++) {
            file1.getline(t1, 100);
            file2.getline(t2, 100);
        }
        //copu node2 tree npde stuff to node1
        //treenode1->readNode(t1);
        treenode2->readNode(t2);
        fs::path filePath = main / t1;
        ofstream nodeFile;
        nodeFile.open(filePath);
        nodeFile << treenode2->key.value << "\n"; //key
        nodeFile << treenode2->height << "\n";
        nodeFile << treenode2->leftChild << "\n"; //left child (if have)
        nodeFile << treenode2->rightChild << "\n"; //right child (if have)
        nodeFile << treenode2->parent << "\n"; //right child (if have)
        int numInstances = treenode2->key.duplicates.size;
        nodeFile << numInstances << "\n";
        for (int i = 0; i < numInstances; i++) {
            nodeFile << treenode2->key.duplicates[i].line << '\n';
        }
        //this->record.writeAllRecords(nodeFile);
        nodeFile.close();
    }

    //rehash

}

void compareMerkelsAVL(str mainMerkel, str branchMerkel, fs::path main, fs::path branch) {

    compareNodesAVL(mainMerkel, branchMerkel, main, branch);


}

//////////////////////////////////GIT COMMANDS
str columns;
void querySession(fs::path dir) {

    cout << "Welcome to youur Gitlite Query Session!\n";
    fs::path qryDir = dir.parent_path();
    cout << "You are working in: " << qryDir << endl;

    str headingsCSV = columns;

    char query[MAX_QUERY_LENGTH] = { 0 };
    bool running = true;
    while (running) {
        cout << "$ ";
        cin.getline(query, MAX_QUERY_LENGTH);
        str qry = query;

        vec<str> query;
        SQL sql(headingsCSV, "sds");
        str q = "s";
        getline(cin, q);
        unsigned int i = 0;
        bool inQuotes = false;
        for (int j = 0; j < q.getSize(); j++) {
            str temp = "";
            if (q[j] == '"') {
                inQuotes = !inQuotes;  // Toggle quote state
                continue;  // Skip the quote character itself
            }

            if (inQuotes) {
                // While in quotes, add everything to current query segment
                query[i] += q[j];
            }
            else if (q[j] != ' ' && q[j] != ';') {
                // Outside quotes, only add non-space/semicolon chars
                query[i] += q[j];
            }
            else {
                // Space or semicolon outside quotes marks end of segment
                i++;
            }
        }
        query.setSize(i);
        for (int j = 0; j < i; j++) {
            cout << query[j] << endl;
        }
        sql.executeQuery(query);

        if (qry == "quit") {
            cout << "...do you wish to commit\n";
            //commit if commit else delete and quit
            char commit[20];
            cin >> commit;
            str cm = commit;
            if (cm == "commit") {
                char commitmsg[MAX_COMMIT_MSG_LENGTH] = { 0 };
                cout << "commit message: ";
                cin >> commitmsg;
                fstream commitFile;
                commitFile.open(dir.parent_path() / fs::path("commits.txt"), ios::app);
                commitFile << commitmsg << "\n";
                commitFile.close();
                fs::path newCommit = dir.parent_path() / commitmsg;
                //rename
                rename(dir, newCommit);
            }
            else {
                fs::remove_all(dir);
            }
            break;
        }
        for (int i = 0; i < 100; i++)
            query[i] = 0;
    }


}

bool hasBOM(ifstream& file) {
    unsigned char bom[3];
    file.read(reinterpret_cast<char*>(bom), 3);
    return (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF);  // UTF-8 BOM
}

Record getColumns(ifstream& dataFile) {
    if (hasBOM(dataFile)) {
        dataFile.seekg(3, ios::beg);  // Skip the BOM
    }
    char line[MAX_RECORD_LENGTH] = { 0 };
    dataFile.getline(line, MAX_RECORD_LENGTH);
    columns = line;
    Record columns;
    columns.insertRecord(line);
    return columns;
}

char* extractColumn(const char* line, int col) {
    char buffer[MAX_RECORD_LENGTH];
    string_copy(buffer, line);
    char* context = nullptr;

    char* token = strtok_s(buffer, ",", &context);
    int index = 0;

    while (token != nullptr) {
        if (index == col) {
            // Allocate memory for the token and copy it
            char* result = new char[cstring_len(token) + 1];
            string_copy(result, token);
            return result; // Return dynamically allocated memory
        }
        token = strtok_s(nullptr, ",", &context);
        index++;
    }

    return nullptr; // Return nullptr if column is not found
}


void loadData(ifstream& dataFile, BTree*& btree, int col) { //btree
    char line[MAX_RECORD_LENGTH] = { 0 };
    while (dataFile.getline(line, sizeof(line))) {
        str column = extractColumn(line, col);
        str key;
        if (isAllDigits(column))
            key = hashInteger(column);
        else
            key = hashString(column);
        btree->insertNode(key, line);
    }
}

void loadData(ifstream& dataFile, AVLTree*& avl, int col) { //avl
    char line[MAX_RECORD_LENGTH] = { 0 };
    while (dataFile.getline(line, sizeof(line))) {
        str column = extractColumn(line, col);
        str key;
        if (isAllDigits(column))
            key = hashInteger(column);
        else
            key = hashString(column);
        avl->insertNode(key, line);
    }
}

void loadData(ifstream& dataFile, RedBlackTree*& rbtree, int col) { //rb
    char line[MAX_RECORD_LENGTH] = { 0 };
    while (dataFile.getline(line, sizeof(line))) {
        str column = extractColumn(line, col);
        str key;
        if (isAllDigits(column))
            key = hashInteger(column);
        else
            key = hashString(column);
        rbtree->insert(Stoi(key), line);
    }
}

void makeAVL(AVLTree* avl, fs::path directory, ifstream& dataFile, int hashAlg, int col) {
    avl = new AVLTree(directory);
    //create nodes
    loadData(dataFile, avl, col);
    //create metadata file
    fstream treeMeta;
    treeMeta.open(directory / fs::path("metadata.txt"), ios::out);
    treeMeta << avl->directory << "\n";
    treeMeta << avl->rootPath << "\n";
    treeMeta.close();
    //merkle
    fs::path merkelDir = directory;
    MerkelTree merkle(avl, merkelDir);
    fs::path meta = directory / "merkleroot.txt";
    fstream file1;
    file1.open(meta, ios::out);
    file1 << merkle.leaf[0]->thisPath << endl;
    file1.close();
    //merkle
}


void makeRedBlack(RedBlackTree* rbtree, fs::path directory, ifstream& dataFile, int hashAlg, int col) { //assume hashC is instructor hash rn
    rbtree = new RedBlackTree(directory);
    //create nodes
    loadData(dataFile, rbtree, col);
    // create metadata file
    fstream treeMeta;
    treeMeta.open(directory / fs::path("metadata.txt"), ios::out);
    treeMeta << rbtree->directory << "\n";
    treeMeta << rbtree->treeRoot << "\n";
    treeMeta << rbtree->nodeCount << "\n";
    treeMeta.close();
}

void makeBTree(BTree* btree, fs::path directory, ifstream& dataFile, int hashAlg, int col) { //assume hashC is instructor hash rn
    int m;
    cout << "m= ";
    cin >> m;
    btree = new BTree(directory, m);
    //create nodes
    loadData(dataFile, btree, col);
    // create metadata file
    fstream treeMeta;
    treeMeta.open(directory / fs::path("metadata.txt"), ios::out);
    treeMeta << btree->directory << "\n";
    treeMeta << btree->rootPath << "\n";
    treeMeta << btree->m << "\n";
    treeMeta << btree->numNodes << "\n";
    treeMeta.close();
}

void initRepository(fs::path& currDir) {

    AVLTree* avl = nullptr;
    BTree* btree = nullptr;
    RedBlackTree* redblack = nullptr;

    //repository folder
    fs::path repo;
    cout << "Repository name(25 chars max): ";
    cin >> repo;
    if (fs::exists(repo)) {
        cout << "repository already exists!\n";
        return;
    }
    fs::create_directories(repo);

    //ask for tree type
    int tree;
    do {
        cout << "Choose A Tree:\n1.AVL\n2.RedBlack\n3.BTree\n";
        cin >> tree;
    } while (!(tree == 1 || tree == 2 || tree == 3));
    //create metadata.txt and mention tree type
    fstream repoMeta;
    repoMeta.open(repo / fs::path("metadata.txt"), ios::out);
    repoMeta << to_string(tree) << "\n";
    //upload csv
    fs::path dataFilePath;
    cout << "upload file: ";
    cin >> dataFilePath;
    ifstream dataFile; //read
    dataFile.open(dataFilePath, ios::binary);
    if (!dataFile.is_open()) {
        cout << "cannot find file!\n";
        return;
    }
    //get columns in the csv
    Record columnList = getColumns(dataFile);
    //ask user which column they want?
    columnList.display();
    int col;
    do {
        cout << "choose a column to hash: ";
        cin >> col;
    } while (col < 0 || col >= columnList.numColoumns);
    //pick a hashing algo
    int hashAlg = 0; //1 for instructor,2 for sha-256
    do {
        cout << "choose a hashing algo (1. Instructor hash , 2. SHA-256):  ";
        cin >> hashAlg;
    } while (!(hashAlg == 1 || hashAlg == 2));
    //write chosen col,hash algo and all columns into metadata and save
    repoMeta << col << "\n";
    repoMeta << hashAlg << "\n";
    repoMeta << columnList.numColoumns << "\n";
    for (int i = 0; i < columnList.numColoumns; i++) {
        repoMeta << columnList[i].c_str() << "\n";
    }
    repoMeta.close();

    //initialise a starting repo
    //create a main branch, with an initial commit
    //ask for commit message
    repo /= fs::path("main");
    fs::create_directories(repo);
    fstream commitFile;
    commitFile.open(repo / fs::path("commits.txt"), ios::out | ios::app);
    char commitmsg[MAX_COMMIT_MSG_LENGTH] = { 0 };
    cout << "commit message: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(commitmsg, MAX_COMMIT_MSG_LENGTH);
    commitFile << 1 << "\n"; //num commit
    commitFile << commitmsg << "\n";
    commitFile.close();
    //repo /= fs::path("main")/commitmsg;
    repo /= commitmsg;

    //read the data file and make the tree
    if (tree == 1)
        makeAVL(avl, repo, dataFile, hashAlg, col);
    else if (tree == 2)
        makeRedBlack(redblack, repo, dataFile, hashAlg, col);
    else if (tree == 3)
        makeBTree(btree, repo, dataFile, hashAlg, col);
    dataFile.close();



    //rename temp
    //fs::rename(repo/fs::path("temp"), repo/ commitmsg);
    currDir = repo;
}


/////////////////////////////////////// main


void duplicateFolder(const fs::path& source, const fs::path& destination) {
    try {
        // Ensure the source exists and is a directory
        if (!fs::exists(source) || !fs::is_directory(source)) {
            throw std::runtime_error("Source folder does not exist or is not a directory.");
        }

        // Create the destination directory if it does not exist
        if (!fs::exists(destination)) {
            fs::create_directories(destination);
        }

        // Iterate through the source directory
        for (const auto& entry : fs::directory_iterator(source)) {
            const fs::path& sourcePath = entry.path();
            fs::path destinationPath = destination / sourcePath.filename();

            if (fs::is_directory(sourcePath)) {
                // Create the subdirectory in the destination
                fs::create_directories(destinationPath);
                // Recursively duplicate the subdirectory
                duplicateFolder(sourcePath, destinationPath);
            }
            else if (fs::is_regular_file(sourcePath)) {
                // Copy the file to the destination
                fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
            }
            else {
                // Handle other file types if needed (e.g., symlinks)
                std::cerr << "Skipping non-regular file: " << sourcePath << '\n';
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}