#include <stdio.h>
#include <stdlib.h>
#include <bool.h>

#define DEGREE 3 // Minimum degree of the B-Tree

typedef struct BTreeNode {
    int *keys;                   
    struct BTreeNode **children; 
    int n;                       
    bool isLeaf;                 
} BTreeNode;

BTreeNode *createNode(bool isLeaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->keys = (int *)malloc((2 * DEGREE - 1) * sizeof(int));
    node->children = (BTreeNode **)malloc(2 * DEGREE * sizeof(BTreeNode *));
    node->n = 0;
    node->isLeaf = isLeaf;
    return node;
}

void traverse(BTreeNode *node) {
    if (node == NULL) return;

    for (int i = 0; i < node->n; i++) {
        if (!node->isLeaf) traverse(node->children[i]);
        printf("%d ", node->keys[i]);
    }
    if (!node->isLeaf) traverse(node->children[node->n]);
}


BTreeNode *search(BTreeNode *node, int key) {
    int i = 0;
    while (i < node->n && key > node->keys[i]) i++;

    if (i < node->n && key == node->keys[i]) return node;

    if (node->isLeaf) return NULL;

    return search(node->children[i], key);
}

void splitChild(BTreeNode *parent, int i, BTreeNode *child) {
    BTreeNode *newChild = createNode(child->isLeaf);
    newChild->n = DEGREE - 1;

    for (int j = 0; j < DEGREE - 1; j++) 
        newChild->keys[j] = child->keys[j + DEGREE];

    if (!child->isLeaf) {
        for (int j = 0; j < DEGREE; j++) 
            newChild->children[j] = child->children[j + DEGREE];
    }

    child->n = DEGREE - 1;

    for (int j = parent->n; j >= i + 1; j--) 
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = newChild;

    for (int j = parent->n - 1; j >= i; j--) 
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = child->keys[DEGREE - 1];
    parent->n++;
}

void insertNonFull(BTreeNode *node, int key) {
    int i = node->n - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;

        if (node->children[i + 1]->n == 2 * DEGREE - 1) {
            splitChild(node, i + 1, node->children[i + 1]);

            if (key > node->keys[i + 1]) i++;
        }
        insertNonFull(node->children[i + 1], key);
    }
}

void insert(BTreeNode **root, int key) {
    BTreeNode *r = *root;

    if (r->n == 2 * DEGREE - 1) {
        BTreeNode *s = createNode(false);
        s->children[0] = r;
        splitChild(s, 0, r);
        insertNonFull(s, key);
        *root = s;
    } else {
        insertNonFull(r, key);
    }
}


// Deletion

void removeFromLeaf(BTreeNode *node, int idx) {
    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];
    node->n--;
}

void removeFromNonLeaf(BTreeNode *node, int idx) {
    int key = node->keys[idx];

    if (node->children[idx]->n >= DEGREE) {
        BTreeNode *cur = node->children[idx];
        while (!cur->isLeaf) cur = cur->children[cur->n];
        int pred = cur->keys[cur->n - 1];
        node->keys[idx] = pred;
        deleteKey(node->children[idx], pred);
    } else if (node->children[idx + 1]->n >= DEGREE) {
        BTreeNode *cur = node->children[idx + 1];
        while (!cur->isLeaf) cur = cur->children[0];
        int succ = cur->keys[0];
        node->keys[idx] = succ;
        deleteKey(node->children[idx + 1], succ);
    } else {
        merge(node, idx);
        deleteKey(node->children[idx], key);
    }
}

void merge(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];

    child->keys[DEGREE - 1] = node->keys[idx];

    for (int i = 0; i < sibling->n; i++)
        child->keys[i + DEGREE] = sibling->keys[i];

    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->n; i++)
            child->children[i + DEGREE] = sibling->children[i];
    }

    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];

    for (int i = idx + 2; i <= node->n; i++)
        node->children[i - 1] = node->children[i];

    child->n += sibling->n + 1;
    node->n--;

    free(sibling);
}

void fill(BTreeNode *node, int idx) {
    if (idx != 0 && node->children[idx - 1]->n >= DEGREE)
        borrowFromPrev(node, idx);
    else if (idx != node->n && node->children[idx + 1]->n >= DEGREE)
        borrowFromNext(node, idx);
    else {
        if (idx != node->n)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

void borrowFromPrev(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx - 1];

    for (int i = child->n - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];

    if (!child->isLeaf) {
        for (int i = child->n; i >= 0; i--)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = node->keys[idx - 1];

    if (!child->isLeaf)
        child->children[0] = sibling->children[sibling->n];

    node->keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n++;
    sibling->n--;
}

void borrowFromNext(BTreeNode *node, int idx) {
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];

    child->keys[child->n] = node->keys[idx];

    if (!child->isLeaf)
        child->children[child->n + 1] = sibling->children[0];

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; i++)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->isLeaf) {
        for (int i = 1; i <= sibling->n; i++)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->n++;
    sibling->n--;
}

void deleteKey(BTreeNode *node, int key) {
    int idx = 0;
    while (idx < node->n && node->keys[idx] < key) idx++;

    if (idx < node->n && node->keys[idx] == key) {
        if (node->isLeaf)
            removeFromLeaf(node, idx);
        else
            removeFromNonLeaf(node, idx);
    } else {
        if (node->isLeaf) {
            printf("The key %d is not in the tree\n", key);
            return;
        }

        bool flag = (idx == node->n);

        if (node->children[idx]->n < DEGREE)
            fill(node, idx);

        if (flag && idx > node->n)
            deleteKey(node->children[idx - 1], key);
        else
            deleteKey(node->children[idx], key);
    }
}

void delete(BTreeNode **root, int key) {
    if (!(*root)) {
        printf("The tree is empty\n");
        return;
    }

    deleteKey(*root, key);

    if ((*root)->n == 0) {
        BTreeNode *tmp = *root;
        if ((*root)->isLeaf)
            *root = NULL;
        else
            *root = (*root)->children[0];

        free(tmp);
    }
}


int main() {
    BTreeNode *root = createNode(true);

    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 5);
    insert(&root, 6);
    insert(&root, 12);
    insert(&root, 30);
    insert(&root, 7);
    insert(&root, 17);

    printf("Traversal of the B-Tree:\n");
    traverse(root);
    printf("\n");

    int key = 6;
    if (search(root, key) != NULL)
        printf("Key %d found in the tree.\n", key);
    else
        printf("Key %d not found in the tree.\n", key);

    return 0;
}
