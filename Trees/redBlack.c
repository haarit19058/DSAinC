#include <stdlib.h>
#include <stdio.h>

/*
Properties of red black trees
1) Each node is either red or black
2) Root is always black
3) Red nodes cannot have red children
4) Every path from a black node ot leaves has same number of black nodes
5) All leaves are black

Derive that the max height of the tree is <=2log(base2)(n+1)
worst possible case is all blacks on one side of root and half black and half red on other side of the tree



*/


typedef enum { RED, BLACK } NodeColor;

typedef struct Node {
    int data;
    NodeColor color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
} Node;

typedef struct RBTree {
    Node* root;
} RBTree;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = RED; 
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// Left rotation to fix violations during insertion
void leftRotate(RBTree* tree, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Right rotation to fix violations during insertion
void rightRotate(RBTree* tree, Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

// Fix the Red-Black Tree after insertion
void fixInsert(RBTree* tree, Node* z) {
    while (z != tree->root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right; // Uncle
            if (y != NULL && y->color == RED) {
                // Case 1: Uncle is RED
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // Case 2: Uncle is BLACK
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left; // Uncle
            if (y != NULL && y->color == RED) {
                // Case 1: Uncle is RED
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // Case 2: Uncle is BLACK
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// Insert a node into the Red-Black Tree
void insert(RBTree* tree, int data) {
    Node* z = createNode(data);
    Node* y = NULL;
    Node* x = tree->root;

    while (x != NULL) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == NULL) {
        tree->root = z;
    } else if (z->data < y->data) {
        y->left = z;
    } else {
        y->right = z;
    }

    fixInsert(tree, z);
}

// Search for a node in the Red-Black Tree
Node* search(RBTree* tree, int data) {
    Node* current = tree->root;
    while (current != NULL && current->data != data) {
        if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return current;
}

// Inorder Traversal to display the tree (for debugging)
void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d(%s) ", root->data, root->color == RED ? "R" : "B");
        inorder(root->right);
    }
}

// Main function
int main() {
    RBTree tree;
    tree.root = NULL;

    insert(&tree, 10);
    insert(&tree, 20);
    insert(&tree, 30);
    insert(&tree, 15);

    printf("Inorder Traversal of the Red-Black Tree:\n");
    inorder(tree.root);
    printf("\n");

    int searchValue = 15;
    Node* foundNode = search(&tree, searchValue);
    if (foundNode != NULL) {
        printf("Node with value %d found.\n", foundNode->data);
    } else {
        printf("Node with value %d not found.\n", searchValue);
    }

    return 0;
}
