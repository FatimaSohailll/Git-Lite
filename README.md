# Git-Lite

GitLite is a simplified version control system built in C++, inspired by Git, that utilizes tree-based data structures to manage versioning, ensure data integrity, and reduce redundancy in data transfer. Our Git stores each tree node in a separate file to minimize memory usage. Key functionalities include repository initialization, branching, switching branches, commit history, and efficient data transfer using Merkle Trees.

---

## Features
**Core Functionalities:**
	•	Initialize Repository (init): 
    Set up a new repository using tree structures such as AVL, B-Tree, or Red-Black Tree.

  •	Commit Changes (commit):
    Save changes to the repository with a custom message.

  •	Branching (branch):
    Create and manage multiple branches for parallel development.

  •	Switch Branches (checkout):
    Navigate between branches seamlessly.

  •	View Commit History (log):
    Access the history of commits for the current branch.
    
  •	Merge Branches (merge):
    Merge changes from one branch into another.

  •	Save and Load Repository: 
    Save the repository state to a file and reload it later.

  • Hashing Techniques:
    Hashing techniques such as **SHA-256** and a custom **Instructor Hash** are used.

  • SQL Integration:
    File updates are managed using SQL commands, enabling structured and efficient data manipulation.

---

## Repository Commands
1.**Initialize Repository:**
    init <filename>
	•	Select a tree type (AVL, B-Tree, or Red-Black Tree).
	•	Choose a CSV column for tree construction.

2.**Commit Changes:**
   commit "message"

3.**Branch Operations:**
	•	*Create a branch:*
    branch <branch_name>
    
  •	*Switch to a branch:*
    checkout <branch_name>
    
  •	*List branches:*
    branches

  •	*Delete a branch:*
    delete-branch <branch_name>

  •	*Merge branches:*
    merge <source_branch> <target_branch>

4.**View Logs:**
    log

5.**Save and Load:**
	•	*Save repository:*
    save

  •	*Load repository:*
    load <file_name>

## Prerequisites

To compile and run GitLite, ensure your system meets the following requirements:

- A C++ compiler supporting C++17 or later.
- CSV dataset

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

## Acknowledgments

- Inspired by Git.
- Special thanks to the contributors who helped build this project.

---
