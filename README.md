# dataStructure
Description
In this problem, you are required to implement Fibonacci Heap. Your implementation should
support insertion, deletion, decrease-key, and extract-min operations while maintaining the
properties of a Fibonacci Heap. After executing all commands, the program should output:
● The final structure of the Fibonacci Heap, printed in level-order traversal, where:
○ Nodes in the same tree and at the same level are separated by a space.
○ Nodes belonging to different trees in the root list are printed on separate lines.
Input Format
The input consists of a series of commands, each on a new line:
● insert <key>: Insert a key into the Fibonacci Heap.
● delete <key>: Delete a key from the Fibonacci Heap.
● decrease <key> <value>: Reduce the value of an existing key by value.
● extract-min: Remove the current minimum key from the heap.
● exit: Terminate the input sequence and end the program.
The commands will be executed sequentially, and the program will stop processing once the
exit command is encountered.
The properties of the Fibonacci Heap and the operations can be referenced from the lecture
slides.
Notes:
● Keys are positive integers and satisfy 1 <= key <= 10^4.
● Keys are unique within the Fibonacci Heap.
● The total number of commands q satisfies 1 <= q <= 100. Commands are executed
sequentially.
● No operation will attempt to delete or decrease a key that does not exist.
● Operations such as extract-min will only be executed when the heap is non-empty.
● The command before the exit operation must be an extract-min operation.
● The Fibonacci Heap must maintain its properties:
○ The minimum pointer (min) always points to the smallest key.
○ Consolidation is performed after every extract-min and delete operation.
○ During consolidation, when multiple roots have the same degree, the two roots
with the smallest key values are merged first and the children of the root of the
new tree should be sorted in ascending order(from left to right).
○ Cascading cuts are performed during delete, decrease-key operations, if
necessary.
Output Format
After processing all commands, output the final Fibonacci Heap structure in level-order
traversal:
line.
⚫ For each tree in the root list, the result of the level-order traversal is printed in a single
⚫ Start with the tree of a smaller degree (e.g. degree = 0 → degree = 1 → …).
Example 1
Sample Input:
insert 10
insert 20
insert 5
insert 30
insert 25
extract-min
decrease 30 22
insert 15
insert 12
extract-min
delete 12
extract-min
exit
Sample Output:
25
15 20
Explanation:
1. Input Commands and Heap Evolution:
● insert 10: Add 10 as the only node.
● insert 20: Add 20. The minimum remains 10. The root list becomes [10, 20].
● insert 5: Add 5. The minimum pointer is updated to 5. The root list becomes
[10, 20, 5].
● insert 30: Add 30. The minimum remains 5. The root list becomes [10, 20, 5,
30].
● insert 25: Add 25. The minimum remains 5. The root list becomes [10, 20, 5,
30, 25].
Struct:
10 — 20 — 5 — 30 — 25
2. extract-min Operation:
● Remove the minimum value 5.
● No children for node 5, so the root list remains [10, 20, 30, 25].
● Perform consolidation (1):
○ Merge 10 and 20 (degree 0 trees) → 10 becomes the root, 20 becomes
its child.
○ Merge 30 and 25 (degree 0 trees) → 25 becomes the root, 30 becomes
its child.
○ Root list becomes [10, 25], with min pointing to 10.
● Perform consolidation (2):
○ Merge 10 and 25 (degree 1 trees) → 10 becomes the root, 20 , 25
becomes its child.
Struct:
10 — 25 10
/ / → / \
20 30 20 25
/
30
3. decrease 30 22 Operation:
● The key 30 is reduced to 8.
● Since 8 violates the heap order property, it is cut from its parent 25 and moves
to the root list.
● Perform cascading cuts (none in this case, as 25 does not lose its second
child).
● Root List: [10, 8].
● The minimum pointer is updated to 8.
Struct:
10 — 8
/ \
20 25
4. insert 15 and insert 12 Operations:
● Insert 15 → Root list becomes [10, 8, 15].
● Insert 12 → Root list becomes [10, 8, 15, 12].
Struct:
10 — 8 — 15 — 12
/ \
20 25
5. extract-min Operation:
● Removes and outputs the minimum value 8.
● Perform consolidation:
○ Merge 15 and 12 (degree 0 trees) → 12 becomes the root, 15 becomes
its child.
○ Root list becomes [10, 12], with min pointing to 10.
Struct:
10 — 12
/ \ /
20 25 15
6. delete 12 Operation:
● Deletes the node with key 12 and 15 moves to the root list .
● Root list becomes [10, 15].
Struct:
10 — 15
/ \
20 25
7. extract-min Operation:
● Removes the minimum value 10.
● Perform consolidation:
○ 20 and 25 move to the root list. Root list becomes [15, 20, 25].
○ Merge 15 and 20 (degree 0 trees) → 15 becomes the root, 20 becomes
its child.
○ Root list becomes [15, 25], with min pointing to 15.
Struct:
15 — 20 — 25 15 — 25
→ /
20
8. The final result of the heap is:
(Degree 0):
25
(Degree 1):
15 20
Example 2
Sample Input:
insert 10
insert 20
insert 5
insert 3
insert 7
insert 15
insert 18
insert 22
insert 1
insert 12
extract-min
decrease 20 1
insert 25
insert 30
extract-min
decrease 30 10
extract-min
decrease 25 4
insert 8
insert 9
extract-min
insert 1
extract-min
extract-min
exit
Sample Output:
9
10 12 20 21 15 18 22 19
