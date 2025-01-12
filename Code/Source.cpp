//INCLUDE
#define NOMINMAX
#define NOCRYPT
#define WIN32_LEAN_AND_MEAN

#include "GitCommands.h"

#include <Windows.h>

int main(int argc, char* argv[]) {

    //BTree* btree = nullptr; //BTree**

    cout << "\033[38;5;166mWelcome to GitLite!\n\n\033[0m";

    bool running = true;
    char input[MAX_COMMAND_LENGTH] = { 0 };
    fs::path currentDir = fs::path("no branch open!");


    while (running) {

        cout << "> ";
        cin.getline(input, MAX_COMMAND_LENGTH);

        str command(input);

        //INIT COMMAND
        if (command == "init") {

            currentDir = fs::path("/");//root
            initRepository(currentDir);


        }
        else if (command == "current-branch") {
            fs::path currBranch = currentDir.parent_path().filename();
            cout << currBranch << endl;
        }
        else if (command == "log") {
            fs::path logpath = currentDir.parent_path();
            fstream logFile;
            logFile.open(logpath / fs::path("commits.txt"));
            char commitmsg[MAX_COMMIT_MSG_LENGTH] = { 0 };
            logFile.getline(commitmsg, MAX_COMMIT_MSG_LENGTH);
            while (!logFile.eof()) {
                logFile.getline(commitmsg, MAX_COMMIT_MSG_LENGTH);
                cout << commitmsg << endl;
            }
            logFile.close();
        }
        else if (command.rfind("branch ", 0, 7) == 0) {
            str branchName = command.substr(7); // Extract branch name
            if (branchName == "") {
                cout << "error: branch name cannot be empty.\n";
                continue;
            }
            fs::path newBranchPath = currentDir.parent_path().parent_path() / branchName.c_str();
            if (fs::exists(newBranchPath)) {
                cout << "error: branch " << branchName << " already exists.\n";
            }
            else {
                duplicateFolder(currentDir.parent_path().parent_path() / fs::path("main"), newBranchPath);
                fstream commitFile;
                commitFile.open(newBranchPath / fs::path("commits.txt"));
                commitFile << "1" << "\n"; //??
                for (const auto& entry : fs::directory_iterator(newBranchPath)) {
                    if (fs::is_directory(entry) && entry.path().filename() != "merkle") {
                        commitFile << entry.path().filename().string() << "\n";
                    }
                }
                commitFile.close();
                cout << "branch " << branchName << " created successfully.\n";
            }

        }
        else if (command.rfind("checkout ", 0, 9) == 0) {
            str branchName = command.substr(9); // Extract branch name
            if (branchName == "") {
                cout << "error: branch name cannot be empty.\n";
                continue;
            }
            fs::path switchBranchPath = currentDir.parent_path().parent_path() / branchName.c_str();
            if (!fs::exists(switchBranchPath)) {
                cout << "error: branch " << branchName << " doesnt exist.\n";
            }
            else {
                //go to latest commit
                currentDir = switchBranchPath;
                fstream commitFile;
                commitFile.open(switchBranchPath / fs::path("commits.txt"));
                char latestCommit[MAX_COMMIT_MSG_LENGTH];
                while (commitFile.getline(latestCommit, MAX_COMMIT_MSG_LENGTH)) {
                    currentDir = switchBranchPath / fs::path(latestCommit);
                }
                cout << "switched to " << currentDir.parent_path() << endl;
            }
        }
        else if (command == "branches") {
            fs::path repo = currentDir.parent_path().parent_path();
            for (const auto& entry : fs::directory_iterator(repo)) {
                if (fs::is_directory(entry.status())) {
                    std::cout << " -- " << entry.path().filename().string() << '\n';
                }
            }
        }
        else if (command.rfind("delete ", 0, 7) == 0) {
            str branchName = command.substr(7); // Extract branch name
            fs::path del = currentDir.parent_path().parent_path() / branchName.c_str();
            if (del == currentDir.parent_path()) {//deleting current branch
                currentDir = currentDir.parent_path().parent_path() / fs::path("main");
                //go to latest commit
                fstream commitFile;
                commitFile.open(currentDir / fs::path("commits.txt"));
                char latestCommit[MAX_COMMIT_MSG_LENGTH];
                while (commitFile.getline(latestCommit, MAX_COMMIT_MSG_LENGTH)) {
                    currentDir = currentDir / fs::path(latestCommit);
                }
                fs::remove_all(del);
                cout << "deleting current branch ... back to main branch\n";
            }
            else
                fs::remove_all(del);
        }
        else if (command == "merge") {
            fs::path branch;
            cout << "branch to merge?: ";
            cin >> branch;
            if (!fs::exists(currentDir.parent_path().parent_path() / branch)) {
                cout << "no such branch!";
                break;
            }
            //get latest commmits
            fstream commitFile;
            fs::path pathMain;
            commitFile.open(currentDir.parent_path().parent_path() / fs::path("main") / fs::path("commits.txt"));
            char latestCommit[MAX_COMMIT_MSG_LENGTH];
            while (commitFile.getline(latestCommit, MAX_COMMIT_MSG_LENGTH)) {
                pathMain = currentDir.parent_path().parent_path() / fs::path("main") / fs::path(latestCommit);
            }
            commitFile.close();
            fs::path pathBranch;
            commitFile.open(currentDir.parent_path().parent_path() / fs::path(branch) / fs::path("commits.txt"));
            //char latestCommit[MAX_COMMIT_MSG_LENGTH];
            while (commitFile.getline(latestCommit, MAX_COMMIT_MSG_LENGTH)) {
                pathBranch = currentDir.parent_path().parent_path() / branch / fs::path(latestCommit);
            }
            //get merkle1 root
            fstream file1, file2;
            char roots[20];
            str root1;
            file1.open(pathMain / fs::path("merkleroot.txt"));
            file1.getline(roots, 20);
            root1 = roots;
            file1.close();
            //root 2
            str root2;
            file2.open(pathBranch / fs::path("merkleroot.txt"));
            file2.getline(roots, 20);
            root2 = roots;
            file2.close();
            compareMerkelsAVL(root1, root2, pathMain, pathBranch);
        }
        else if (command == "query") {
            //duplicate and call querysession
            fs::path tmp = currentDir.parent_path() / fs::path("temp");
            duplicateFolder(currentDir, tmp); //create tmp di for commit
            querySession(tmp);
            //execute query
        }



        for (int i = 0; i < 100; i++)
            input[i] = 0;

        Sleep(1000);

    }
    return 0;
}
