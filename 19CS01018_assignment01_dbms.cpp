/*
    Assignment 1 (DBMS)
    Name: Divyansh Agrawal
    RollNo: 19CS01018
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <csignal>
using namespace std;

template <typename T>
class Node {
  public:
    bool isLeaf;
    vector<T> keys;
    Node *ptr2next;
    vector<Node<T> *> ptr2Tree;
    vector<vector<string>> dataPtr;
    Node() {
        this->isLeaf = false;
        this->ptr2next = NULL;
    }
};

template <typename T>
class BPTree {
  public:
    int degree;
    Node<T> *root;
    bool isPrimaryKey;
    Node<T> *parent = NULL; // for function findParent (which is recursive)
    
    void insertInternal(T x, Node<T> **cursor, Node<T> **child) {
        if(binary_search((*cursor)->keys.begin(),(*cursor)->keys.end(),x)) 
            return;
        if ((*cursor)->keys.size() < degree - 1) {
            int i = upper_bound((*cursor)->keys.begin(),(*cursor)->keys.end(),x) - (*cursor)->keys.begin();
            (*cursor)->keys.push_back(x);
            (*cursor)->ptr2Tree.push_back(*child);

            if (i != (*cursor)->keys.size() - 1) {
                for (int j = (*cursor)->keys.size() - 1; j > i; j--) {
                    (*cursor)->keys[j] = (*cursor)->keys[j - 1];
                }
                for (int j = (*cursor)->ptr2Tree.size() - 1; j > (i + 1); j--) {
                    (*cursor)->ptr2Tree[j] = (*cursor)->ptr2Tree[j - 1];
                }
                (*cursor)->keys[i] = x;
                (*cursor)->ptr2Tree[i + 1] = *child;
            }
        }
        else {
            vector<T> virtualKeyNode((*cursor)->keys);
            vector<Node<T> *> virtualTreePtrNode((*cursor)->ptr2Tree);
            int i = upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), x) - (*cursor)->keys.begin();
            virtualKeyNode.push_back(x);
            virtualTreePtrNode.push_back(*child);

            if (i != virtualKeyNode.size() - 1) {
                for (int j = virtualKeyNode.size() - 1; j > i; j--) {
                    virtualKeyNode[j] = virtualKeyNode[j - 1];
                }
                for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
                    virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
                }
                virtualKeyNode[i] = x;
                virtualTreePtrNode[i + 1] = *child;
            }

            T partitionKey;
            partitionKey = virtualKeyNode[(virtualKeyNode.size()/2)];
            int partitionIdx = (virtualKeyNode.size()/2);

            (*cursor)->keys.resize(partitionIdx);
            (*cursor)->ptr2Tree.resize(partitionIdx + 1);
            (*cursor)->ptr2Tree.reserve(partitionIdx + 1);
            for (int i = 0; i < partitionIdx; i++) {
                (*cursor)->keys[i] = virtualKeyNode[i];
            }
            for (int i = 0; i < partitionIdx + 1; i++) {
                (*cursor)->ptr2Tree[i] = virtualTreePtrNode[i];
            }

            Node<T> *newInternalNode = new Node<T>;
            new (&newInternalNode->ptr2Tree) vector<Node<T> *>;
            for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
                newInternalNode->keys.push_back(virtualKeyNode[i]);
            }
            for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {
                newInternalNode->ptr2Tree.push_back(virtualTreePtrNode[i]);
            }

            if ((*cursor) == root) {
                Node<T> *newRoot = new Node<T>;
                newRoot->keys.push_back(partitionKey);
                new (&newRoot->ptr2Tree) vector<Node<T> *>;
                newRoot->ptr2Tree.push_back(*cursor);
                newRoot->ptr2Tree.push_back(newInternalNode);
                root = newRoot;
            }
            else {
                insertInternal(partitionKey, findParent(root, *cursor), &newInternalNode);
            }
        }
    }
    Node<T> **findParent(Node<T> *cursor, Node<T> *child) {
        if (cursor->isLeaf || cursor->ptr2Tree[0]->isLeaf)
            return NULL;

        for (int i = 0; i < cursor->ptr2Tree.size(); i++) {
            if (cursor->ptr2Tree[i] == child) {
                parent = cursor;
                return &parent;
            }
            else {
                // Commenting To Remove vector out of bound Error:
                // new (&cursor->ptr2Tree) vector<Node*>;
                Node<T> *tmpCursor = cursor->ptr2Tree[i];
                findParent(tmpCursor, child);
            }
        }
        return &parent;
    }
    Node<T> *firstLeftNode(Node<T> *cursor) {
        if (cursor->isLeaf)
            return cursor;
        for (int i = 0; i < cursor->ptr2Tree.size(); i++)
            if (cursor->ptr2Tree[i] != NULL)
                return firstLeftNode(cursor->ptr2Tree[i]);
        return NULL;
    }

public:
    BPTree() {
        this->degree = 8; // default degree
        this->root = NULL;
        this->isPrimaryKey = true; // default key is primary
    }
    BPTree(int degree, bool isPrimaryKey) {
        this->degree = degree;
        this->root = NULL;
        this->isPrimaryKey = isPrimaryKey;
    }
    Node<T> *getRoot() {
        return this->root;
    }
    int getdegree() {
        return degree;
    }
    void setRoot(Node<T> *ptr) {
        this->root = ptr;
    }

    void bpt_print(Node<T> *cursor) {
        if (!cursor) {
            cout << "No Data in the Database yet!" << endl;
            return;
        }
        cout << "\n#########################################################################\n";
        cout << "[root] -> ";
        print(cursor, 1);
        cout << "#########################################################################\n";
    }
    void print(Node<T> *cursor, int tabs) {
        if (cursor != NULL) {
            for (auto i : cursor->keys) {
                cout << i << " ";
            }
            cout << endl;

            if (!cursor->isLeaf) {
                for (int i = 0; i < (cursor->keys).size() + 1; ++i) {
                    for(int j = 0; j < tabs; j++) {
                        cout << '\t';
                    }
                    if(i == 0) {
                        cout << "[..., " << cursor->keys[i] << "] -> ";
                    }
                    else if(i == (cursor->keys.size())) {
                        cout << '[' << cursor->keys[i-1] << ", ...] -> ";
                    }
                    else {
                        cout << '[' << cursor->keys[i-1] << ", " << cursor->keys[i] << "] -> ";
                    }
                    print(cursor->ptr2Tree[i], tabs+1);
                }
            }
        }
    }
    void seqDisplay(Node<T> *cursor) {
        if (!cursor) {
            cout << "No Data in the Database yet!" << endl;
            return;
        }
        Node<T> *firstLeft = firstLeftNode(cursor);
        if (!firstLeft) {
            cout << "No Data in the Database yet!" << endl;
            return;
        }
        while (firstLeft) {
            for (int i = 0; i < firstLeft->keys.size(); i++) {
                cout << firstLeft->keys[i] << " ";
            }
            firstLeft = firstLeft->ptr2next;
        }
        cout << endl;
    }
    
    void removeAll(Node<T> *cursor) {
        if (!cursor) {
            return;
        }
        Node<T> *firstLeft = firstLeftNode(cursor);
        if (!firstLeft) {
            return;
        }
        while (firstLeft) {
            for (auto i : firstLeft->dataPtr) {
                for(string fN : i) {
                    remove(fN.c_str());
                }
            }
            firstLeft = firstLeft->ptr2next;
        }
        cout << endl;
    }

    void search(T key) {
        if (root == NULL) {
            cout << "No data inserted yet" << endl;
            return;
        }
        else {
            Node<T> *cursor = root;
            while (cursor->isLeaf == false) {
                int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor = cursor->ptr2Tree[idx];
            }

            if(!binary_search(cursor->keys.begin(), cursor->keys.end(), key)) {
                cout << "The requested data was not found" << endl;
                return;
            }

            int idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

            if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
                cout << "The requested data was not found" << endl;
                return;
            }

            for(auto fileName : cursor->dataPtr[idx]) {
                FILE *filePtr = fopen(fileName.c_str(), "r");
                char ch = fgetc(filePtr);
                while (ch != EOF) {
                    printf("%c", ch);
                    ch = fgetc(filePtr);
                }
                fclose(filePtr);
                // cout << endl;
            }
        }
    }

    void searchInRange(T start, T end) {
        if (root == NULL) {
            cout << "No data inserted yet" << endl;
            return;
        }
        else {
            Node<T> *cursor = root;
            while (cursor->isLeaf == false) {
                int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), start) - cursor->keys.begin();
                cursor = cursor->ptr2Tree[idx];
            }

            int idx = lower_bound(cursor->keys.begin(), cursor->keys.end(), start) - cursor->keys.begin();

            if (idx == cursor->keys.size()) {   
                cout << "The requested data was not found" << endl;
                return;
            }

            while(cursor) {
                for(int i = idx; i < cursor->keys.size(); i++) {
                    if(cursor->keys[i] > end) {
                        return;
                    }
                    for(string fileName : cursor->dataPtr[i]) {
                        FILE *filePtr = fopen(fileName.c_str(), "r");
                        char ch = fgetc(filePtr);
                        while (ch != EOF) {
                            printf("%c", ch);
                            ch = fgetc(filePtr);
                        }
                        fclose(filePtr);
                    }
                }
                idx = 0;
                cursor = cursor->ptr2next;
            } 
        }
    }

    void insert(T key, string fileName) {
        if (root == NULL) {
            root = new Node<T>;
            root->isLeaf = true;
            root->keys.push_back(key);
            new (&root->dataPtr) vector<vector<string>>;
            vector<string> temp;
            temp.push_back(fileName);
            root->dataPtr.push_back(temp);
        }
        else {
            Node<T> *cursor = root;
            Node<T> *parent = NULL;
            while (cursor->isLeaf == false) {
                parent = cursor;
                int idx = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor = cursor->ptr2Tree[idx];
            }

            if(binary_search(cursor->keys.begin(), cursor->keys.end(), key)) {
                if(isPrimaryKey) {
                    cout << "Error: Duplicate value on the same index can not be inserted for primary key" << endl;
                }
                else {
                    int i = lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                    cursor->dataPtr[i].push_back(fileName);
                    // cout << "Duplicate value inserted in same index as the indexing is based on non-primary key" << endl;
                }
            }
            else if (cursor->keys.size() < degree) {
                int i = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
                cursor->keys.push_back(key);
                vector<string> temp;
                temp.push_back(fileName);
                cursor->dataPtr.push_back(temp);

                if (i != cursor->keys.size() - 1) {
                    for (int j = cursor->keys.size() - 1; j > i; j--) {
                        cursor->keys[j] = cursor->keys[j - 1];
                        cursor->dataPtr[j] = cursor->dataPtr[j - 1];
                    }
                    cursor->keys[i] = key;
                    cursor->dataPtr[i] = temp;
                }
            }
            else {
                vector<T> virtualNode(cursor->keys);
                vector<vector<string>> virtualDataNode(cursor->dataPtr);

                int i = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

                virtualNode.push_back(key);
                vector<string> temp;
                temp.push_back(fileName);
                virtualDataNode.push_back(temp);

                if (i != virtualNode.size() - 1) {
                    for (int j = virtualNode.size() - 1; j > i; j--) {
                        virtualNode[j] = virtualNode[j - 1];
                        virtualDataNode[j] = virtualDataNode[j - 1];
                    }
                    virtualNode[i] = key;
                    virtualDataNode[i] = temp;
                }

                Node<T> *newLeaf = new Node<T>;
                newLeaf->isLeaf = true;
                new (&newLeaf->dataPtr) vector<vector<string>>;
                
                Node<T> *tmp = cursor->ptr2next;
                cursor->ptr2next = newLeaf;
                newLeaf->ptr2next = tmp;

                cursor->keys.resize((degree) / 2 + 1);
                cursor->dataPtr.reserve((degree) / 2 + 1);
                for (int i = 0; i <= (degree) / 2; i++) {
                    cursor->keys[i] = virtualNode[i];
                    cursor->dataPtr[i] = virtualDataNode[i];
                }
                for (int i = (degree) / 2 + 1; i < virtualNode.size(); i++) {
                    newLeaf->keys.push_back(virtualNode[i]);
                    newLeaf->dataPtr.push_back(virtualDataNode[i]);
                }

                if (cursor == root) {
                    Node<T> *newRoot = new Node<T>;
                    newRoot->keys.push_back(newLeaf->keys[0]);
                    new (&newRoot->ptr2Tree) vector<Node<T> *>;
                    newRoot->ptr2Tree.push_back(cursor);
                    newRoot->ptr2Tree.push_back(newLeaf);
                    root = newRoot;
                }
                else {
                    insertInternal(newLeaf->keys[0], &parent, &newLeaf);
                }
            }
        }
    }
    void removeKey(T key) {
        Node<T> *root = getRoot();
        if (root == NULL) {
            cout << "B+ tree is empty" << endl;
            return;
        }
        Node<T> *cursor = root;
        Node<T> *parent;
        int leftSibling, rightSibling;

        while (cursor->isLeaf != true) {
            for (int i = 0; i < cursor->keys.size(); i++) {
                parent = cursor;
                leftSibling = i - 1;
                rightSibling = i + 1;

                if (key < cursor->keys[i]) {
                    cursor = cursor->ptr2Tree[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    leftSibling = i;
                    rightSibling = i + 2;
                    cursor = cursor->ptr2Tree[i + 1];
                    break;
                }
            }
        }

        int pos = 0;
        bool found = false;
        for (pos = 0; pos < cursor->keys.size(); pos++) {
            if (cursor->keys[pos] == key) {
                found = true;
                break;
            }
        }

        auto itr = lower_bound(cursor->keys.begin(), cursor->keys.end(), key);

        if (itr == cursor->keys.end()) {
            cout << "The requested key was not found in the tree" << endl;
            return;
        }
        
        int idx = itr - cursor->keys.begin();
        for(auto fileName : cursor->dataPtr[idx]) {
            // delete cursor->dataPtr[pos];//avoid memory leaks
            if (remove(fileName.c_str()) == 0)
                cout << "Successfully deleted file: " << fileName << endl;
            else
                cout << "Unable to delete the file: " << fileName << endl;
        }
        
        for (int i = pos; i < cursor->keys.size() - 1; i++) {
            cursor->keys[i] = cursor->keys[i + 1];
            cursor->dataPtr[i] = cursor->dataPtr[i + 1];
        }
        int prev_size = cursor->keys.size();
        cursor->keys.resize(prev_size - 1);
        cursor->dataPtr.resize(prev_size - 1);

        if (cursor == root) {
            if (cursor->keys.size() == 0) {
                setRoot(NULL);
            }
        }

        cout << "Deleted " << key << " successfully" << endl;
        if (cursor->keys.size() >= (getdegree() + 1) / 2) {
            return;
        }
        cout << "Starting redistribution as underflow in the leaf node occured" << endl;

        if (leftSibling >= 0) {
            Node<T> *leftNode = parent->ptr2Tree[leftSibling];
            if (leftNode->keys.size() >= (getdegree() + 1) / 2 + 1) {
                int maxIdx = leftNode->keys.size() - 1;
                cursor->keys.insert(cursor->keys.begin(), leftNode->keys[maxIdx]);
                cursor->dataPtr.insert(cursor->dataPtr.begin(), leftNode->dataPtr[maxIdx]);
                leftNode->keys.resize(maxIdx);
                leftNode->dataPtr.resize(maxIdx);
                parent->keys[leftSibling] = cursor->keys[0];
                return;
            }
        }

        if (rightSibling < parent->ptr2Tree.size()) {
            Node<T> *rightNode = parent->ptr2Tree[rightSibling];
            if (rightNode->keys.size() >= (getdegree() + 1) / 2 + 1) {
                int minIdx = 0;
                cursor->keys.push_back(rightNode->keys[minIdx]);
                cursor->dataPtr.push_back(rightNode->dataPtr[minIdx]);
                rightNode->keys.erase(rightNode->keys.begin());
                rightNode->dataPtr.erase(rightNode->dataPtr.begin());
                parent->keys[rightSibling - 1] = rightNode->keys[0];
                return;
            }
        }

        if (leftSibling >= 0) {
            Node<T> *leftNode = parent->ptr2Tree[leftSibling];
            for (int i = 0; i < cursor->keys.size(); i++) {
                leftNode->keys.push_back(cursor->keys[i]);
                leftNode->dataPtr.push_back(cursor->dataPtr[i]);
            }
            leftNode->ptr2next = cursor->ptr2next;
            removeInternal(parent->keys[leftSibling], parent, cursor);
        }
        else if (rightSibling <= parent->keys.size()) {
            Node<T> *rightNode = parent->ptr2Tree[rightSibling];
            for (int i = 0; i < rightNode->keys.size(); i++) {
                cursor->keys.push_back(rightNode->keys[i]);
                cursor->dataPtr.push_back(rightNode->dataPtr[i]);
            }
            cursor->ptr2next = rightNode->ptr2next;
            removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
        }
    }
    void removeInternal(T x, Node<T> *cursor, Node<T> *child) {
        Node<T> *root = getRoot();

        if (cursor == root) {
            if (cursor->keys.size() == 1) {
                if (cursor->ptr2Tree[1] == child) {
                    setRoot(cursor->ptr2Tree[0]);
                    return;
                }
                else if (cursor->ptr2Tree[0] == child) {
                    setRoot(cursor->ptr2Tree[1]);
                    return;
                }
            }
        }

        int pos;
        for (pos = 0; pos < cursor->keys.size(); pos++) {
            if (cursor->keys[pos] == x) {
                break;
            }
        }
        for (int i = pos; i < cursor->keys.size() - 1; i++) {
            cursor->keys[i] = cursor->keys[i + 1];
        }
        cursor->keys.resize(cursor->keys.size() - 1);

        for (pos = 0; pos < cursor->ptr2Tree.size(); pos++) {
            if (cursor->ptr2Tree[pos] == child) {
                break;
            }
        }

        for (int i = pos; i < cursor->ptr2Tree.size() - 1; i++) {
            cursor->ptr2Tree[i] = cursor->ptr2Tree[i + 1];
        }
        cursor->ptr2Tree.resize(cursor->ptr2Tree.size() - 1);

        if (cursor->keys.size() >= (getdegree() + 1) / 2 - 1) {
            cout << "Deleted " << x << " successfully\n";
            return;
        }
        cout << "Starting redistribution as underflow in the leaf node occured" << endl;
        if (cursor == root) {
            return;
        }

        Node<T> **p1 = findParent(root, cursor);
        Node<T> *parent = *p1;

        int leftSibling, rightSibling;
        for (pos = 0; pos < parent->ptr2Tree.size(); pos++) {
            if (parent->ptr2Tree[pos] == cursor) {
                leftSibling = pos - 1;
                rightSibling = pos + 1;
                break;
            }
        }

        if (leftSibling >= 0) {
            Node<T> *leftNode = parent->ptr2Tree[leftSibling];
            if (leftNode->keys.size() >= (getdegree() + 1) / 2) {
                int maxIdxKey = leftNode->keys.size() - 1;
                cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
                parent->keys[leftSibling] = leftNode->keys[maxIdxKey];

                int maxIdxPtr = leftNode->ptr2Tree.size() - 1;
                cursor->ptr2Tree.insert(cursor->ptr2Tree.begin(), leftNode->ptr2Tree[maxIdxPtr]);
                leftNode->keys.resize(maxIdxKey);
                leftNode->dataPtr.resize(maxIdxPtr);
                return;
            }
        }

        if (rightSibling < parent->ptr2Tree.size()) {
            Node<T> *rightNode = parent->ptr2Tree[rightSibling];
            if (rightNode->keys.size() >= (getdegree() + 1) / 2) {
                int maxIdxKey = rightNode->keys.size() - 1;
                cursor->keys.push_back(parent->keys[pos]);
                parent->keys[pos] = rightNode->keys[0];
                rightNode->keys.erase(rightNode->keys.begin());
                cursor->ptr2Tree.push_back(rightNode->ptr2Tree[0]);
                cursor->ptr2Tree.erase(cursor->ptr2Tree.begin());
                return;
            }
        }

        if (leftSibling >= 0) {
            Node<T> *leftNode = parent->ptr2Tree[leftSibling];
            leftNode->keys.push_back(parent->keys[leftSibling]);
            for (auto val : cursor->keys) {
                leftNode->keys.push_back(val);
            }
            for (int i = 0; i < cursor->ptr2Tree.size(); i++) {
                leftNode->ptr2Tree
                    .push_back(cursor->ptr2Tree[i]);
                cursor->ptr2Tree[i] = NULL;
            }
            cursor->ptr2Tree.resize(0);
            cursor->keys.resize(0);
            removeInternal(parent->keys[leftSibling], parent, cursor);
        }
        else if (rightSibling < parent->ptr2Tree.size()) {
            Node<T> *rightNode = parent->ptr2Tree[rightSibling];
            cursor->keys.push_back(parent->keys[rightSibling - 1]);
            for (auto val : rightNode->keys) {
                cursor->keys.push_back(val);
            }
            for (int i = 0; i < rightNode->ptr2Tree.size(); i++) {
                cursor->ptr2Tree
                    .push_back(rightNode->ptr2Tree[i]);
                rightNode->ptr2Tree[i] = NULL;
            }
            rightNode->ptr2Tree.resize(0);
            rightNode->keys.resize(0);
            removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
        }
    }
};

template <typename T>
void insertionEmployee(BPTree<T> **bPTree) {
    int employeeId;
    int age, salary;
    string name;

    cout << "\nEnter the EmployeeID, FirstName, Age and Salary of the employee: ";
    cin >> employeeId >> name >> age >> salary;
    string fileName = "database/employees/";
    fileName += to_string(employeeId) + ".txt";

    bool isUpdate = 0;
    if(FILE *file = fopen(fileName.c_str(), "r")) {
        fclose(file);
        cout << "This employee id already exists in the database.\n\tDo you want to update the data? (1 for yes/ 0 for no) : ";
        cin >> isUpdate;
        if(!isUpdate) {
            return;
        }
    }

    FILE *filePtr = fopen(fileName.c_str(), "w");
    string userTuple = to_string(employeeId) + " " + name + " " + to_string(age) + " " + to_string(salary) + "\n";
    fprintf(filePtr, userTuple.c_str());
    fclose(filePtr);

    if(isUpdate) {
        cout << "Update successful" << endl;
    }
    else {
        (*bPTree)->insert(employeeId, fileName); // for primary key indexing
        // (*bPTree)->insert(age, fileName); // for non-primary key indexing
        // (*bPTree)->insert(name, fileName); // for non-primary key indexing
        cout << "Insertion of employee id: " << employeeId << " was successful" << endl;
    }
}

string gen_random(int len) {
    string s;
    s.resize(len);
    s[0] = 'A' + rand()%26;
    for (int i = 1; i < len; i++) {
        s[i] = 'a' + rand()%26;
    }
    return s;
}

template <typename T>
void autoInsertionEmployee(BPTree<T> **bPTree) {
    int numEmployees;
    cout << "Enter the number of employees to be inserted in the database: ";
    cin >> numEmployees;

    int employeeId, age, salary;
    string name;
    for(int i = 0; i < numEmployees; i++) {
        employeeId = (i+1)*10;
        age = 21 + rand()%20;
        salary = 10000 + 500*(rand()%100);
        name = gen_random(2);

        string fileName = "database/employees/";
        fileName += to_string(employeeId) + ".txt";
        FILE *filePtr = fopen(fileName.c_str(), "w");
        string userTuple = to_string(employeeId) + " " + name + " " + to_string(age) + " " + to_string(salary) + "\n";
        fprintf(filePtr, userTuple.c_str());
        fclose(filePtr);

        (*bPTree)->insert(employeeId, fileName); // for primary key indexing
        // (*bPTree)->insert(age, fileName); // for non-primary key indexing
        // (*bPTree)->insert(name, fileName); // for non-primary key indexing
    }
}

template <typename T>
void searchKey(BPTree<T> *bPTree) {
    T key;
    cout << "Enter the key to Search: ";
    cin >> key;
    cout << endl;
    bPTree->search(key);
}

template <typename T>
void searchInRange(BPTree<T> *BPTree) {
    T start, end;
    cout << "Enter the starting index: ";
    cin >> start;
    cout << "Enter the ending index: ";
    cin >> end;
    BPTree->searchInRange(start, end);
}

template <typename T>
void displaySequential(BPTree<T> *bPTree) {
    bPTree->seqDisplay(bPTree->getRoot());
}

template <typename T>
void displayDepthwise(BPTree<T> *bPTree) {
    bPTree->bpt_print(bPTree->getRoot());
}

template <typename T>
void deleteKey(BPTree<T> *bPTree) {
    T key;
    cout << "Enter a key to delete: ";
    cin >> key;
    bPTree->removeKey(key);
}

template <typename T>
void deleteAllEmployees(BPTree<T> *bPTree) {
    bPTree->removeAll(bPTree->getRoot());
}

int main() {
    bool flag = true;
    int option;
    int degree = 8;
    bool isPrimaryKey = true;
    cout << "Please enter the degree of the B+ tree: ";
    cin >> degree;
    cout << "If the indexing is to be done on primary key then enter '1' else '0': ";
    cin >> isPrimaryKey;
    BPTree<int> *bPTree = new BPTree<int>(degree, isPrimaryKey);

    while (flag) {
        cout << "\nPress --\n\t1: Insert one tuple at a time\n\t2: Insert multiple tuples with random values\n\t3: Search a tuple\n\t4: Search In Range\n\t5: Sequential display of current tree\n\t6: Depthwise display of current tree\n\t7: Delete a key from the Tree\n\t8 or other: Exit" << endl;
        cin >> option;
        switch (option) {
        case 1:
            insertionEmployee(&bPTree);
            break;
        case 2:
            autoInsertionEmployee(&bPTree);
            break;
        case 3:
            searchKey(bPTree);
            break;
        case 4:
            searchInRange(bPTree);
            break;
        case 5:
            displaySequential(bPTree);
            break;
        case 6:
            displayDepthwise(bPTree);
            break;
        case 7:
            deleteKey(bPTree);
            break;
        default:
            deleteAllEmployees(bPTree);
            flag = false;
            break;
        }
    }
    return 0;
}
