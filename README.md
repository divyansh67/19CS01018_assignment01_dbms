# README
## _Database Server_

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://github.com/divyansh67/19CS01018_assignment01_dbms)

This is a code for a database server which uses B+ tree to store and process data.
The code is implemented in C++.

## Features

- Insert a single tuple
- Insert multiple random tuples in one go
- Search for tuple(s) by index (range can also be given).
- Display the B+ tree in sequential or depthwise mode.
- Delete a key by giving index.


## Features

- Every tuple is stored in the database as a separate text file.
- The B+ tree stores its filename to access it.
- B+ tree's memory is dynamically allocated on creation.
- Leaf nodes are linked in a linked list.
- Actual filepath for the data is stored only on leaf nodes.
- Indexing can be done in two ways: (Primary key/ Non-primary key).
- For primary key, one node can store max degree number of tuples.
- For non-primary key, one node can store tuples which are represented by max degree number of indices.
- On exiting, the program deletes all the database files it created.

## Setup
- Clone the repository.
- Go to the folder containing the code file.
- Make sure the folder has another folder in it named *database*.
- The *database* folder should have the folder *employee*.
- Now run this command in your command prompt.
```sh 
g++ 19CS01018_assignment01_dbms.cpp -o bplustree
./bplustree
```
- *bplustree* is the executable file. You can name it whatever you wish.
- All the data will be stored in the *database* folder.

## Implementation details
| Function | Implementation |
| ------ | ------ |
| Insert | If the node has an empty space, insert the key/reference pair into the node. If the node is already full, split it into two nodes, distributing the keys evenly between the two nodes. If the node is a leaf, take a copy of the minimum value in the second of these two nodes and repeat this insertion algorithm to insert it into the parent node. If the node is a non-leaf, exclude the middle value during the split and repeat this insertion algorithm to insert this excluded value into the parent node. |
| Remove | Check if the value exists in this leaf node. Delete the respective File and FILE*. Shift the keys and dataPtr for the leaf Node. If sufficient Node is not available for invariant to hold then try to borrow a key from leftSibling. Transfer the maximum key from the left Sibling and resize the left Sibling Node After Tranfer. Else try to borrow a key from rightSibling.
| AutoInsertion | Make an employee Id by mutiplying the iterator number with 10. Take a random age in between 21 and 40. Salary in between 10000 and 60000. Generate a random name of length 5 (you should change it to <3 if indexing is to be done on name). Create a file named _employeeId.txt_ and store the data in the file. Store the key in the B+ tree. |
| displaySequential | Print the current B+ tree in a sequential mode (increasing index) by iterating through the linked list implemented over the leaf nodes. |
| displayDepthwise | Print the B+ tree in the depth-first-search format with each tab representing the number of levels the data is below the root. This function does this by calling another function which is a recursive one. |

## Time-complexity

- Consider _**d**_ = degree
- And _**n**_ = number of keys
- The number of disk operations is the dominating factor of the time demanded in practice.
- Most of the time gets consumed in reading and writing in the disk. However, considering it constant, we can come up with a time-complexity of:

| Operation | Complexity |
| ------ | ------ |
| Insert a tuple | O ( d * log_d(n) ) |
| Search a tuple | O ( log2(d) * log_d(n) ) |
| Search in range | O ( k * log2(d) * log_d(n) ) |
| Sequential display | O ( n ) |
| Depthwise display | O ( n ) |
| Delete a key | O ( d * log_d(n) ) |# 19CS01018_assignment01_dbms
