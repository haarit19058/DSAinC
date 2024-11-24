// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>

// #define INPUT "/home/haarit/Desktop/MasteringC/DSA1/input.txt"
// #define OUTPUT "/home/haarit/Desktop/MasteringC/DSA1/output.txt"
// // #define headers "/home/haarit/Desktop/MasteringC/DSA1/headers/all.h"
// // #include headers

// typedef struct nodeavl{
//     int val;
//     int height;
//     struct nodeavl* left;
//     struct nodeavl* right;
//     struct nodeavl* parent;
// } nodeavl;

// void inorder(nodeavl* root) {
//     if (root == NULL) {
//         return;
//     }
//     inorder(root->left);
//     printf("%d ", root->val);
//     inorder(root->right);
// }

// nodeavl* createNode() {
//     nodeavl* node = (nodeavl*)malloc(sizeof(nodeavl));
//     node->val = -1;
//     node->height = 1;
//     node->left = NULL;
//     node->right = NULL;
//     node->parent = NULL;
//     return node;
// }

// void insertBst(nodeavl* root, int data) {
//     if (data <= root->val) {
//         if (root->left == NULL) {
//             nodeavl* temp = createNode();
//             root->left = temp;
//             temp->parent = root;
//             temp->val = data;
//         } else {
//             insertBst(root->left, data);
//         }
//     } else {
//         if (root->right == NULL) {
//             nodeavl* temp = createNode();
//             root->right = temp;
//             temp->parent = root;
//             temp->val = data;
//         } else {
//             insertBst(root->right, data);
//         }
//     }
// }

// int findMaxHeight(nodeavl* root) {
//     if (root == NULL) {
//         return 0;
//     }
//     int left = findMaxHeight(root->left);
//     int right = findMaxHeight(root->right);
//     return (left > right ? left : right) + 1;
// }

// void findHeightInorder(nodeavl* root) {
//     if (root == NULL) {
//         return;
//     }
//     findHeightInorder(root->left);
//     printf("%d ", findMaxHeight(root));
//     findHeightInorder(root->right);
// }

// int checkImbalance(nodeavl* tempnode) {
//     if (abs(findMaxHeight(tempnode->right) - findMaxHeight(tempnode->left)) <= 1) {
//         return 0;  // Balanced
//     }
//     return 1;  // Imbalanced
// }


// void rotateRR(nodeavl* node) {
//     nodeavl* newRoot = node->left;
//     node->left = newRoot->right;
//     if (newRoot->right != NULL) newRoot->right->parent = node;

//     newRoot->right = node;
//     newRoot->parent = node->parent;
//     node->parent = newRoot;

//     if (newRoot->parent) {
//         if (newRoot->parent->left == node) newRoot->parent->left = newRoot;
//         else newRoot->parent->right = newRoot;
//     }
// }

// void rotateLL(nodeavl* node) {
//     nodeavl* newRoot = node->right;
//     node->right = newRoot->left;
//     if (newRoot->left != NULL) newRoot->left->parent = node;

//     newRoot->left = node;
//     newRoot->parent = node->parent;
//     node->parent = newRoot;

//     if (newRoot->parent) {
//         if (newRoot->parent->left == node) newRoot->parent->left = newRoot;
//         else newRoot->parent->right = newRoot;
//     }
// }

// void rotateLR(nodeavl* node) {
//     nodeavl* leftChild = node->left;
//     nodeavl* leftRightChild = leftChild->right;

//     leftChild->right = leftRightChild->left;
//     if (leftRightChild->left != NULL) {
//         leftRightChild->left->parent = leftChild;
//     }

//     leftRightChild->left = leftChild;
//     leftChild->parent = leftRightChild;

//     node->left = leftRightChild;
//     leftRightChild->parent = node;

//     rotateRR(node);
// }

// void rotateRL(nodeavl* node) {
//     nodeavl* rightChild = node->right;
//     nodeavl* rightLeftChild = rightChild->left;

//     rightChild->left = rightLeftChild->right;
//     if (rightLeftChild->right != NULL) {
//         rightLeftChild->right->parent = rightChild;
//     }

//     rightLeftChild->right = rightChild;
//     rightChild->parent = rightLeftChild;

//     node->right = rightLeftChild;
//     rightLeftChild->parent = node;

//     rotateLL(node);
// }

// void balance_helper(nodeavl* node) {
//     int balanceFactor = findMaxHeight(node->left) - findMaxHeight(node->right);

//     if (balanceFactor > 1) { // Left heavy
//         if (findMaxHeight(node->left->left) >= findMaxHeight(node->left->right)) {
//             rotateRR(node); // Right rotate
//         } else {
//             rotateLR(node); // Left-right rotate
//         }
//     } else if (balanceFactor < -1) { // Right heavy
//         if (findMaxHeight(node->right->right) >= findMaxHeight(node->right->left)) {
//             rotateLL(node); // Left rotate
//         } else {
//             rotateRL(node); // Right-left rotate
//         }
//     }
// }

// void balance(nodeavl* leafNode) {
//     nodeavl* temp = leafNode;
//     while (temp != NULL) {
//         if (checkImbalance(temp)) {
//             balance_helper(temp);
//             // break;
//         }
//         temp = temp->parent;
//     }

//     // printf("%d ",temp->val);
// }


// nodeavl* findMin(nodeavl* root) {
//     while (root->left != NULL) {
//         root = root->left;
//     }
//     return root;
// }

// // nodeavl* deleteNode(nodeavl* root, int data) {
// //     if (root == NULL) {
// //         return root;
// //     }
// //     if (data < root->val) {
// //         root->left = deleteNode(root->left, data);
// //     } else if (data > root->val) {
// //         root->right = deleteNode(root->right, data);
// //     } else {
// //         // Node to be deleted found
// //         if (root->left == NULL) {
// //             nodeavl* temp = root->right;
// //             free(root);
// //             return temp;
// //         } else if (root->right == NULL) {
// //             nodeavl* temp = root->left;
// //             free(root);
// //             return temp;
// //         }

// //         // Node with two children: Get the inorder successor
// //         nodeavl* temp = findMin(root->right);

// //         root->val = temp->val;

// //         // Delete the inorder successor
// //         root->right = deleteNode(root->right, temp->val);
// //     }

// //     // Update the height of the current node
// //     root->height = findMaxHeight(root);

// //     // Balance the tree
// //     balance(root);

// //     return root;
// // }

// int main() {
//     nodeavl* root = createNode();
//     root->val = 13;

//     insertBst(root, 5);
//     insertBst(root, 10);
//     insertBst(root, 11);
//     insertBst(root, 15);

//     printf("Max Height of the tree is %d \n", findMaxHeight(root));

//     balance(root->left->right->right);
//     inorder(root);
//     printf("\n");

//     findHeightInorder(root);
//     printf("\n\n");


//     // printf("Deleting node 10\n");
//     // root = deleteNode(root, 10);

//     inorder(root);
//     printf("\n");

//     return 0;
// }








/* Given by chatgpt */



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
