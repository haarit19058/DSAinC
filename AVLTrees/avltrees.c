#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct nodeavl {
    int val;
    int height;
    struct nodeavl* left;
    struct nodeavl* right;
    struct nodeavl* parent;
} nodeavl;

nodeavl* createNode(int data) {
    nodeavl* node = (nodeavl*)malloc(sizeof(nodeavl));
    node->val = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

int height(nodeavl* node) {
    return node ? node->height : 0;
}

void updateHeight(nodeavl* node) {
    if (node) {
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }
}

void rotateRR(nodeavl** root) {
    nodeavl* newRoot = (*root)->left;
    (*root)->left = newRoot->right;
    if (newRoot->right) newRoot->right->parent = *root;

    newRoot->right = *root;
    newRoot->parent = (*root)->parent;
    (*root)->parent = newRoot;

    updateHeight(*root);
    updateHeight(newRoot);
    *root = newRoot;
}

void rotateLL(nodeavl** root) {
    nodeavl* newRoot = (*root)->right;
    (*root)->right = newRoot->left;
    if (newRoot->left) newRoot->left->parent = *root;

    newRoot->left = *root;
    newRoot->parent = (*root)->parent;
    (*root)->parent = newRoot;

    updateHeight(*root);
    updateHeight(newRoot);
    *root = newRoot;
}

void rotateLR(nodeavl** root) {
    rotateLL(&(*root)->left);
    rotateRR(root);
}

void rotateRL(nodeavl** root) {
    rotateRR(&(*root)->right);
    rotateLL(root);
}

void balance(nodeavl** root) {
    updateHeight(*root);
    int balanceFactor = height((*root)->left) - height((*root)->right);

    if (balanceFactor > 1) {
        if (height((*root)->left->left) >= height((*root)->left->right)) {
            rotateRR(root); // Right rotate
        } else {
            rotateLR(root); // Left-right rotate
        }
    } else if (balanceFactor < -1) {
        if (height((*root)->right->right) >= height((*root)->right->left)) {
            rotateLL(root); // Left rotate
        } else {
            rotateRL(root); // Right-left rotate
        }
    }
}

nodeavl* insertBst(nodeavl* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->val) {
        root->left = insertBst(root->left, data);
        root->left->parent = root;
    } else if (data > root->val) {
        root->right = insertBst(root->right, data);
        root->right->parent = root;
    }

    balance(&root);
    return root;
}

void inorder(nodeavl* root) {
    if (root) {
        inorder(root->left);
        printf("%d ", root->val);
        inorder(root->right);
    }
}

int findMaxHeight(nodeavl* root) {
    return root ? root->height : 0;
}

int main() {
    nodeavl* root = NULL;
    
    root = insertBst(root, 13);
    root = insertBst(root, 5);
    root = insertBst(root, 10);
    root = insertBst(root, 11);
    root = insertBst(root, 15);

    printf("Max Height of the tree is %d\n", findMaxHeight(root));
    inorder(root);
    printf("\n");

    return 0;
}
