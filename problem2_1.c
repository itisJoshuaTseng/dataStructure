#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct FibNode
{
    int key;
    int degree;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
    int mark;
} FibNode;

typedef struct FibonacciHeap
{
    FibNode *min;
    int n;
} FibonacciHeap;

FibNode *createNode(int key)
{
    FibNode *node = (FibNode *)malloc(sizeof(FibNode));
    node->key = key;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->left = node;
    node->right = node;
    node->mark = 0;
    return node;
}

FibonacciHeap *createHeap()
{
    FibonacciHeap *heap = (FibonacciHeap *)malloc(sizeof(FibonacciHeap));
    heap->min = NULL;
    heap->n = 0;
    return heap;
}

void insert(FibonacciHeap *heap, int key)
{
    FibNode *node = createNode(key);
    if (heap->min == NULL)
    {
        heap->min = node;
        node->left = node;
        node->right = node;
    }
    else
    {
        node->right = heap->min->right;
        node->left = heap->min;
        heap->min->right->left = node;
        heap->min->right = node;

        if (node->key < heap->min->key)
        {
            heap->min = node;
        }
    }

    heap->n++;
}

void link(FibNode *y, FibNode *x)
{
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if (x->child == NULL)
    {
        x->child = y;
        y->left = y;
        y->right = y;
    }
    else
    {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = 0;
}

int compareKeys(const void *a, const void *b)
{
    FibNode *nodeA = *(FibNode **)a;
    FibNode *nodeB = *(FibNode **)b;
    return nodeA->key - nodeB->key;
}

void consolidate(FibonacciHeap *heap)
{
    int D = (int)(log(heap->n) / log(2)) + 1;
    FibNode *A[D];
    for (int i = 0; i < D; i++)
    {
        A[i] = NULL;
    }

    int rootCount = 0;
    FibNode *current = heap->min;
    FibNode *start = heap->min;

    do
    {
        rootCount++;
        current = current->right;
    } while (current != start);

    FibNode *roots[rootCount];
    current = heap->min;
    for (int i = 0; i < rootCount; i++)
    {
        roots[i] = current;
        current = current->right;
    }

    qsort(roots, rootCount, sizeof(FibNode *), compareKeys);

    for (int i = 0; i < rootCount; i++)
    {
        FibNode *x = roots[i];
        int d = x->degree;

        while (A[d] != NULL)
        {
            FibNode *y = A[d];
            if (x->key > y->key)
            {
                FibNode *temp = x;
                x = y;
                y = temp;
            }
            link(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }

    FibNode *minNode = NULL;
    FibNode *firstNonNull = NULL;
    FibNode *lastNonNull = NULL;

    for (int i = 0; i < D; i++)
    {
        if (A[i] != NULL)
        {
            if (firstNonNull == NULL)
            {
                firstNonNull = A[i];
                firstNonNull->left = firstNonNull;
                firstNonNull->right = firstNonNull;
                minNode = A[i];
            }
            else
            {
                A[i]->left = lastNonNull;
                A[i]->right = firstNonNull;
                lastNonNull->right = A[i];
                firstNonNull->left = A[i];
            }

            lastNonNull = A[i];

            if (minNode == NULL || A[i]->key < minNode->key)
            {
                minNode = A[i];
            }
        }
    }

    if (firstNonNull != NULL && lastNonNull != NULL)
    {
        firstNonNull->left = lastNonNull;
        lastNonNull->right = firstNonNull;
    }

    heap->min = minNode;
}

int extractMin(FibonacciHeap *heap)
{
    FibNode *z = heap->min;
    if (z != NULL)
    {
        if (z->child != NULL)
        {
            FibNode *child = z->child;

            FibNode *childLeft = child->left;
            FibNode *minRight = heap->min->right;

            child->left = heap->min;
            childLeft->right = minRight;
            minRight->left = childLeft;
            heap->min->right = child;

            FibNode *current = child;
            do
            {
                current->parent = NULL;
                current = current->right;
            } while (current != child);
        }

        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right)
        {
            heap->min = NULL;
        }
        else
        {
            heap->min = z->right;
            consolidate(heap);
        }

        heap->n--;
    }

    int minKey = z->key;
    free(z);
    return minKey;
}

void cut(FibonacciHeap *heap, FibNode *x, FibNode *y)
{
    if (x->right == x)
    {
        y->child = NULL;
    }
    else
    {
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x)
        {
            y->child = x->right;
        }
    }
    y->degree--;

    if (heap->min != NULL)
    {
        x->left = heap->min;
        x->right = heap->min->right;
        heap->min->right->left = x;
        heap->min->right = x;
    }
    else
    {
        heap->min = x;
        x->left = x;
        x->right = x;
    }

    x->parent = NULL;
    x->mark = 0;
}

void cascadingCut(FibonacciHeap *heap, FibNode *y)
{
    FibNode *z = y->parent;
    if (z != NULL)
    {
        if (y->mark == 0)
        {
            y->mark = 1;
        }
        else
        {
            cut(heap, y, z);
            cascadingCut(heap, z);
        }
    }
}

FibNode *find(FibNode *node, int key)
{
    FibNode *found = NULL;
    FibNode *start = node;
    if (node != NULL)
    {
        do
        {
            if (node->key == key)
            {
                found = node;
                break;
            }
            if (node->child != NULL)
            {
                found = find(node->child, key);
                if (found != NULL)
                {
                    break;
                }
            }
            node = node->right;
        } while (node != start);
    }
    return found;
}

void decreaseKey(FibonacciHeap *heap, int oldKey, int newKey)
{
    FibNode *x = find(heap->min, oldKey);
    if (x == NULL || newKey > x->key)
    {
        return;
    }
    x->key = newKey;

    FibNode *y = x->parent;
    if (y != NULL && x->key < y->key)
    {
        cut(heap, x, y);
        cascadingCut(heap, y);
    }

    if (x->key < heap->min->key)
    {
        heap->min = x;
    }
}

void delete (FibonacciHeap *heap, int key)
{
    decreaseKey(heap, key, INT_MIN);
    extractMin(heap);
}

int compareChildKeys(const void *a, const void *b)
{
    FibNode *nodeA = *(FibNode **)a;
    FibNode *nodeB = *(FibNode **)b;
    return nodeA->key - nodeB->key;
}

void level_order(FibNode *root)
{
    FibNode *queue[1000];
    queue[0] = root;
    int front = 0;
    int rear = 1;

    while (front < rear)
    {
        FibNode *node = queue[front++];
        printf("%d ", node->key);

        if (node->child != NULL)
        {
            FibNode *child = node->child;
            int childCount = 0;
            FibNode *childArray[100];

            FibNode *currentChild = child;
            do
            {
                childArray[childCount++] = currentChild;
                currentChild = currentChild->right;
            } while (currentChild != child);

            qsort(childArray, childCount, sizeof(FibNode *), compareChildKeys);

            for (int i = 0; i < childCount; i++)
            {
                queue[rear++] = childArray[i];
            }
        }
    }
    printf("\n");
}

void printHeap(FibonacciHeap *heap)
{
    if (heap->min == NULL)
    {
        printf("Heap is empty.\n");
        return;
    }

    int rootCount = 0;
    FibNode *roots[1000];
    FibNode *current = heap->min;

    do
    {
        roots[rootCount++] = current;
        current = current->right;
    } while (current != heap->min);

    qsort(roots, rootCount, sizeof(FibNode *), compareChildKeys);

    int maxDegree = 0;
    for (int i = 0; i < rootCount; i++)
    {
        if (roots[i]->degree > maxDegree)
        {
            maxDegree = roots[i]->degree;
        }
    }

    for (int d = 0; d <= maxDegree; d++)
    {
        bool hasNodes = false;
        for (int i = 0; i < rootCount; i++)
        {
            if (roots[i]->degree == d)
            {
                hasNodes = true;
                level_order(roots[i]);
            }
        }
    }
}

int main()
{
    FibonacciHeap *heap = createHeap();
    char command[20];
    int key, value;

    while (1)
    {
        scanf("%s", command);
        if (strcmp(command, "insert") == 0)
        {
            scanf("%d", &key);
            insert(heap, key);
        }
        else if (strcmp(command, "extract-min") == 0)
        {
            int min = extractMin(heap);
        }
        else if (strcmp(command, "decrease") == 0)
        {
            scanf("%d %d", &key, &value);
            decreaseKey(heap, key, key - value);
        }
        else if (strcmp(command, "delete") == 0)
        {
            scanf("%d", &key);
            delete (heap, key);
        }
        else if (strcmp(command, "exit") == 0)
        {
            break;
        }
    }

    if (heap->min != NULL)
    {
        printHeap(heap);
    }

    return 0;
}
