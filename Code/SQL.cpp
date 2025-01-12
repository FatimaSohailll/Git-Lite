#include "SQL.h"

    SQL:: SQL(str headingsCSV, str repo1) : headingsCSV(headingsCSV), repo(repo1) {
        parseHeadings(this->headingsCSV, this->Titles);
    }
    void SQL::parseHeadings(str& headingCSV, vec<str>& Titles) {
        str title = "";
        for (unsigned int i = 0; i < headingsCSV.getSize(); i++) {
            if (headingsCSV[i] != ',') {
                title += headingsCSV[i];
            }
            else {
                title.push_back('\0');
                title.size--;
                Titles.copy_back(title);
                title = "";
            }
        }
        if (title != "") {
            title.push_back('\0');
            title.size--;
            Titles.copy_back(title);
        }
    }

    void SQL::executeQuery(vec<str>& query) {
        if (query.getSize() == 0) {
            cout << "\ninvalid query...\n";
            return;
        }
        if (query[0] == "SELECT") {
            //execute select query[0]
            select(query);
        }
        else if (query[0] == "UPDATE") {
            //execute update query[0]
            update(query);
        }
        else if (query[0] == "DELETE") {
            //execute delete query[0]
            Delete(query);
        }
        else if (query[0] == "INSERT") {
            //execute insert query[0]
            insert(query);
        }
        else {
            cout << "\ninvalid command...\n";
            return;
        }
    }

    void SQL::select(vec<str>& query) {
        //execute select query
        if (query.getSize() < 8) {
            cout << "\ninvalid query...\n";
            return;
        }
        if (query[1] == "*") {

            if (query[2] == "FROM") {
                fs::path branch = fs::path(repo.c_str()) / query[3].c_str();
                if (fs::exists(fs::path(repo.c_str()) / "metadata.txt")) { //path to file
                    ifstream metaData(fs::path(repo.c_str()) / "metadata.txt");
                    //do something
                    cout << "\nBranch found...\n";
                    str treeType, col, hash;
                    getline(metaData, treeType);
                    getline(metaData, col);
                    getline(metaData, hash);
                    metaData.close();
                    if (query[4] == "WHERE") {
                        //do something
                        str column = query[5];
                        //check if column exists
                        if (Titles.getSize() == 0) {
                            cout << "\nColumn not found...\n";
                            return;
                        }
                        else {
                            bool found = false;
                            for (unsigned int i = 0; i < Titles.getSize(); i++) {
                                if (Titles[i] == column) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                cout << "\nColumn not found...\n";
                                return;
                            }

                            //CONTIUNE WHERE
                            if (query[6] == "=") {
                                //do something
                                str value = query[7];
                                ///YAHAN HO GA KAAM
                                ifstream commits(branch / fs::path("commits.txt"));
                                str latestCommit;
                                while (!commits.eof()) {
                                    getline(commits, latestCommit);
                                    str dummdumm;
                                    getline(commits, dummdumm);
                                }
                                latestCommit.size++;
                                commits.close();
                                str commitPath = "commit-";
                                commitPath.append(latestCommit);
                                commitPath.push_back('\0');
                                fs::path selectedCommit = branch / fs::path(commitPath.c_str());
                                ifstream rootFile(selectedCommit / "metadata.txt");
                                str filePrefix;
                                str rootPath;
                                str m;
                                getline(rootFile, filePrefix);
                                getline(rootFile, rootPath);
                                if (treeType == "3") {
                                    getline(rootFile, m);
                                }
                                rootFile.close();
                                str key;
                                //get index of column
                                int WhichColumn = 0;
                                for (unsigned int i = 0; i < Titles.getSize(); i++) {
                                    if (Titles[i] == column) {
                                        col = toString(i);
                                        WhichColumn = i;
                                        break;
                                    }
                                }
                                if (hash == "1") {
                                    if (isAllDigits(value))
                                        key = hashInteger(value);
                                    else
                                        key = hashString(value);
                                }
                                else {
                                    //sha
                                }

                                if (treeType == "1") {
                                    AVLTree tree(selectedCommit);
                                    tree.rootPath = rootPath;
                                    tree.search(key, value, WhichColumn);
                                }
                                if (treeType == "2") {
                                    //RB
                                }
                                if (treeType == "3") {
                                    //BT
                                  /*  BTree tree(selectedCommit, Stoi(m));
                                    tree.rootPath = rootPath;
                                    tree.searchKey(key);*/
                                }
                            }
                            else {
                                cout << "\ninvalid query...\n";
                                return;
                            }
                        }
                    }
                    else {
                        cout << "\ninvalid query...\n";
                        return;
                    }
                }
                else {
                    cout << "\nBranch not found...\n";
                    return;
                }
            }
            else {
                //select specific columns
                cout << "\ninvalid query...\n";
                return;
            }
        }
        else {
            //select specific columns
            cout << "\ninvalid query...\n";
            return;
        }
    }
    void SQL::update(vec<str>& query) {
        //execute update query
        if (query.getSize() < 10) {
            cout << "\ninvalid query...\n";
            return;
        }
        fs::path branch = fs::path(repo.c_str()) / query[1].c_str();
        if (fs::exists(fs::path(repo.c_str()) / "metadata.txt")) { //path to file
            ifstream metaData(fs::path(repo.c_str()) / "metadata.txt");
            //do something
            cout << "\nBranch found...\n";
            str treeType, col, hash;
            getline(metaData, treeType);
            getline(metaData, col);
            getline(metaData, hash);
            metaData.close();
            if (query[2] == "SET") {
                str newValue;
                int newCol;
                str columnToChange = query[3];
                //check if column exists
                if (Titles.getSize() == 0) {
                    cout << "\nColumn not found...\n";
                    return;
                }
                else {
                    bool found = false;
                    for (unsigned int i = 0; i < Titles.getSize(); i++) {
                        if (Titles[i] == columnToChange) {
                            found = true;
                            newCol = i;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "\nColumn not found...\n";
                        return;
                    }

                    if (query[4] != "=") {
                        cout << "\ninvalid query...\n";
                        return;
                    }
                    newValue = query[5];
                }

                if (query[6] == "WHERE") {
                    //do something
                    str column = query[7];
                    //check if column exists
                    if (Titles.getSize() == 0) {
                        cout << "\nColumn not found...\n";
                        return;
                    }
                    else {
                        bool found = false;
                        for (unsigned int i = 0; i < Titles.getSize(); i++) {
                            if (Titles[i] == column) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "\nColumn not found...\n";
                            return;
                        }

                        //CONTIUNE WHERE
                        if (query[8] == "=") {
                            //do something
                            str value = query[9];
                            ///YAHAN HO GA KAAM
                            ifstream commits(branch / fs::path("commits.txt"));
                            str latestCommit;
                            while (!commits.eof()) {
                                getline(commits, latestCommit);
                                str dummdumm;
                                getline(commits, dummdumm);
                            }
                            latestCommit.size++;
                            commits.close();
                            str commitPath = "commit-";
                            commitPath.append(latestCommit);
                            commitPath.push_back('\0');
                            fs::path selectedCommit = branch / fs::path(commitPath.c_str());
                            ifstream rootFile(selectedCommit / "metadata.txt");
                            str filePrefix;
                            str rootPath;
                            str m;
                            getline(rootFile, filePrefix);
                            getline(rootFile, rootPath);
                            if (treeType == "3") {
                                getline(rootFile, m);
                            }
                            rootFile.close();
                            str key;
                            //get index of column
                            int WhichColumn = 0;
                            for (unsigned int i = 0; i < Titles.getSize(); i++) {
                                if (Titles[i] == column) {
                                    col = toString(i);
                                    WhichColumn = i;
                                    break;
                                }
                            }
                            if (hash == "1") {
                                if (isAllDigits(value))
                                    key = hashInteger(value);
                                else
                                    key = hashString(value);
                            }
                            else {
                                //sha
                            }
                            str fileToUpdate;
                            if (treeType == "1") {
                                AVLTree tree(selectedCommit);
                                tree.rootPath = rootPath;
                                fileToUpdate = tree.getFilePath(key);
                                AVLupdatethisForThat(selectedCommit / fileToUpdate.c_str(), value, newValue, WhichColumn, newCol);
                            }
                            if (treeType == "2") {
                                //RB
                            }
                            if (treeType == "3") {
                                //BT
                              /*  BTree tree(selectedCommit, Stoi(m));
                                rootPath.push_back('\0');
                                tree.rootPath = rootPath;
                                fileToUpdate = tree.BTgetFilePath(key);
                                BTupdatethisForThat(selectedCommit / fileToUpdate.c_str(), value, newValue, WhichColumn, newCol);*/
                            }

                        }
                        else {
                            cout << "\ninvalid query...\n";
                            return;
                        }
                    }
                }
                else {
                    cout << "\ninvalid query...\n";
                    return;
                }

            }

            else {
                cout << "\nBranch not found...\n";
                return;
            }
        }
    }
    void SQL:: AVLupdatethisForThat(fs::path filename, str& condition, str& newthis, int conditioncol, int thiscol) {
        // Create temp file
        str tempFile = "temp.txt";
        std::ifstream inFile(filename.c_str());
        std::ofstream outFile(tempFile.c_str());

        if (!inFile || !outFile) {
            std::cout << "Error opening files\n";
            return;
        }

        str line;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;

        while (getline(inFile, line)) {
            std::istringstream ss(line.c_str());
            str item;
            str updatedLine;
            bool firstItem = true;
            bool nameFound = false;
            int fieldCount = 0;

            // Process each comma-separated value
            while (getline(ss, item, ',')) {
                if (!firstItem) {
                    updatedLine += ',';
                }

                // If this is the name field and matches
                if (fieldCount == conditioncol && item == condition) {
                    nameFound = true;
                    if (fieldCount == thiscol && nameFound)
                        updatedLine.append(newthis);
                    else
                        updatedLine.append(item);
                }
                // If this is the age field and name was found
                else if (fieldCount == thiscol && nameFound) {
                    updatedLine.append(newthis);
                }
                else {
                    updatedLine.append(item);
                }

                firstItem = false;
                fieldCount++;
            }
            outFile << updatedLine << '\n';
            cout << updatedLine << endl;
        }

        inFile.close();
        outFile.close();

        // Replace original with updated file
        fs::remove(filename.c_str());
        fs::rename(tempFile.c_str(), filename.c_str());
    }
    void SQL::BTupdatethisForThat(fs::path filename, str& condition, str& newthis, int conditioncol, int thiscol) {
        // Create temp file
        str tempFile = "temp.txt";
        std::ifstream inFile(filename.c_str());
        std::ofstream outFile(tempFile.c_str());

        if (!inFile || !outFile) {
            std::cout << "Error opening files\n";
            return;
        }

        str line;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;
        getline(inFile, line);
        outFile << line << '\n';
        cout << line << endl;

        while (getline(inFile, line)) {
            std::istringstream ss(line.c_str());
            str item;
            str updatedLine;
            bool firstItem = true;
            bool nameFound = false;
            int fieldCount = 0;

            // Process each comma-separated value
            while (getline(ss, item, ',')) {
                if (!firstItem) {
                    updatedLine += ',';
                }

                // If this is the name field and matches
                if (fieldCount == conditioncol && item == condition) {
                    nameFound = true;
                    if (fieldCount == thiscol && nameFound)
                        updatedLine.append(newthis);
                    else
                        updatedLine.append(item);
                }
                // If this is the age field and name was found
                else if (fieldCount == thiscol && nameFound) {
                    updatedLine.append(newthis);
                }
                else {
                    updatedLine.append(item);
                }

                firstItem = false;
                fieldCount++;
            }
            outFile << updatedLine << '\n';
            cout << updatedLine << endl;
        }

        inFile.close();
        outFile.close();

        // Replace original with updated file
        fs::remove(filename.c_str());
        fs::rename(tempFile.c_str(), filename.c_str());
    }
    void SQL:: Delete(vec<str>& query) {
        //execute delete query
        if (query.getSize() < 7) {
            cout << "\ninvalid query...\n";
            return;
        }
        if (query[1] == "FROM") {
            fs::path branch = fs::path(repo.c_str()) / query[2].c_str();
            if (fs::exists(fs::path(repo.c_str()) / "metadata.txt")) { //path to file
                ifstream metaData(fs::path(repo.c_str()) / "metadata.txt");
                //do something
                cout << "\nBranch found...\n";
                str treeType, col, hash;
                getline(metaData, treeType);
                getline(metaData, col);
                getline(metaData, hash);
                metaData.close();
                if (query[3] == "WHERE") {
                    //do something
                    str column = query[4];
                    //check if column exists
                    if (Titles.getSize() == 0) {
                        cout << "\nColumn not found...\n";
                        return;
                    }
                    else {
                        bool found = false;
                        for (unsigned int i = 0; i < Titles.getSize(); i++) {
                            if (Titles[i] == column) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "\nColumn not found...\n";
                            return;
                        }

                        //CONTIUNE WHERE
                        if (query[5] == "=") {
                            //do something
                            str value = query[6];
                            ///YAHAN HO GA KAAM
                            ifstream commits(branch / fs::path("commits.txt"));
                            str latestCommit;
                            while (!commits.eof()) {
                                getline(commits, latestCommit);
                                str dummdumm;
                                getline(commits, dummdumm);
                            }
                            latestCommit.size++;
                            commits.close();
                            str commitPath = "commit-";
                            commitPath.append(latestCommit);
                            commitPath.push_back('\0');
                            fs::path selectedCommit = branch / fs::path(commitPath.c_str());
                            ifstream rootFile(selectedCommit / "metadata.txt");
                            str filePrefix;
                            str rootPath;
                            str m;
                            getline(rootFile, filePrefix);
                            getline(rootFile, rootPath);
                            if (treeType == "3") {
                                getline(rootFile, m);
                            }
                            rootFile.close();
                            str key;
                            //get index of column
                            int WhichColumn = 0;
                            for (unsigned int i = 0; i < Titles.getSize(); i++) {
                                if (Titles[i] == column) {
                                    col = toString(i);
                                    WhichColumn = i;
                                    break;
                                }
                            }
                            if (hash == "1") {
                                if (isAllDigits(value))
                                    key = hashInteger(value);
                                else
                                    key = hashString(value);
                            }
                            else {
                                //sha
                            }
                            if (treeType == "1") {
                                AVLTree tree(selectedCommit);
                                tree.rootPath = rootPath;
                                tree.remove(key);
                            }
                            if (treeType == "2") {
                                //RB
                            }
                            if (treeType == "3") {
                                BTree tree(selectedCommit, Stoi(m));
                                tree.rootPath = rootPath;
                                tree.deleteKey(key);

                            }
                        }
                        else {
                            cout << "\ninvalid query...\n";
                            return;
                        }
                    }
                }
                else {
                    cout << "\ninvalid query...\n";
                    return;
                }
            }
            else {
                cout << "\nBranch not found...\n";
                return;
            }
        }
        else {
            //select specific columns
            cout << "\ninvalid query...\n";
            return;
        }
    }


    void SQL:: insert(vec<str>& query) {
        //execute insert query
        if (query.getSize() < 5) {
            cout << "\ninvalid query...\n";
            return;
        }
        if (query[1] == "INTO") {
            fs::path branch = fs::path(repo.c_str()) / query[2].c_str();
            if (fs::exists(fs::path(repo.c_str()) / "metadata.txt")) { //path to file
                ifstream metaData(fs::path(repo.c_str()) / "metadata.txt");
                //do something
                cout << "\nBranch found...\n";
                str treeType, col, hash;
                getline(metaData, treeType);
                getline(metaData, col);
                getline(metaData, hash);
                metaData.close();
                if (query[3] == "VALUE") {
                    //do something
                    str rec = query[4];
                    ///YAHAN HO GA KAAM
                    ifstream commits(branch / fs::path("commits.txt"));
                    str latestCommit;
                    while (!commits.eof()) {
                        getline(commits, latestCommit);
                        str dummdumm;
                        getline(commits, dummdumm);
                    }
                    latestCommit.size++;
                    commits.close();
                    str commitPath = "commit-";
                    commitPath.append(latestCommit);
                    commitPath.push_back('\0');
                    fs::path selectedCommit = branch / fs::path(commitPath.c_str());
                    ifstream rootFile(selectedCommit / "metadata.txt");
                    str filePrefix;
                    str rootPath;
                    str m;
                    getline(rootFile, filePrefix);
                    getline(rootFile, rootPath);
                    if (treeType == "3") {
                        getline(rootFile, m);
                    }
                    rootFile.close();
                    vec<str> forCol;
                    str key;
                    parseHeadings(rec, forCol);
                    if (hash == "1") {
                        if (isAllDigits(forCol[Stoi(col)]))
                            key = hashInteger(forCol[Stoi(col)]);
                        else
                            key = hashString(forCol[Stoi(col)]);
                    }
                    else {
                        //sha
                    }

                    if (treeType == "1") {
                        AVLTree tree(selectedCommit);
                        tree.rootPath = rootPath;
                        tree.insertNode(key, rec);
                    }
                    if (treeType == "2") {
                        //RB
                    }
                    if (treeType == "3") {
                        //B
                        BTree tree(selectedCommit, Stoi(m));
                        tree.rootPath = rootPath;
                        tree.insertNode(key, rec);
                    }
                    cout << "Record inserted successfully...\n";
                }

                else {
                    cout << "\ninvalid query...\n";
                    return;
                }
            }
            else {
                cout << "\nBranch not found...\n";
                return;
            }
        }
        else {
            cout << "\ninvalid query...\n";
            return;
        }
    }
