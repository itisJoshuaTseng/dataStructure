# Description
In this problem, you are required to implement a Fibonacci Heap. Your implementation should support `insertion`, `deletion`, `decrease-key`, and `extract-min` operations while maintaining the properties of a Fibonacci Heap. After executing all commands, the program should output:

- The final structure of the Fibonacci Heap, printed in level-order traversal, where:
  - Nodes in the same tree and at the same level are separated by a space.
  - Nodes belonging to different trees in the root list are printed on separate lines.

## Input Format
The input consists of a series of commands, each on a new line:

- `insert <key>`: Insert a key into the Fibonacci Heap.
- `delete <key>`: Delete a key from the Fibonacci Heap.
- `decrease <key> <value>`: Reduce the value of an existing key by the specified value.
- `extract-min`: Remove the current minimum key from the heap.
- `exit`: Terminate the input sequence and end the program.

The commands will be executed sequentially, and the program will stop processing once the `exit` command is encountered.  
The properties of the Fibonacci Heap and the operations can be referenced from the lecture slides.

## Notes
- Keys are positive integers and satisfy `1 <= key <= 10^4`.
- Keys are unique within the Fibonacci Heap.
- The total number of commands `q` satisfies `1 <= q <= 100`. Commands are executed sequentially.
- No operation will attempt to delete or decrease a key that does not exist.
- Operations such as `extract-min` will only be executed when the heap is non-empty.
- The command before the `exit` operation must be an `extract-min` operation.
- The Fibonacci Heap must maintain its properties:
  - The minimum pointer (`min`) always points to the smallest key.
  - Consolidation is performed after every `extract-min` and `delete` operation.
  - During consolidation, when multiple roots have the same degree, the two roots with the smallest key values are merged first, and the children of the root of the new tree should be sorted in ascending order (from left to right).
  - Cascading cuts are performed during `delete` and `decrease-key` operations, if necessary.

## Output Format
After processing all commands, output the final Fibonacci Heap structure in level-order traversal:

- For each tree in the root list, the result of the level-order traversal is printed in a single line.
- Start with the tree of a smaller degree (e.g., `degree = 0 → degree = 1 → …`).
