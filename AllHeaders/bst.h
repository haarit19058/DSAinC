#ifndef BINARYTREE
#define BINARYTREE

#include "node.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct BST{
    bstnode* head;
    int size;
}BST;

void bst_init(BST* bst){
    bst->head = NULL; // The head should initially be NULL, not an allocated node
    bst->size = 0;
}

// void bst_insert_helper(bstnode* tnode, int data){
//     if (data < tnode->data) {
//         if (tnode->left == NULL) {
//             bstnode* newnode = (bstnode*)malloc(sizeof(bstnode));
//             newnode->data = data;
//             newnode->left = NULL;
//             newnode->right = NULL;
//             tnode->left = newnode;
//         } else {
//             bst_insert_helper(tnode->left, data);
//         }
//     } else {
//         if (tnode->right == NULL) {
//             bstnode* newnode = (bstnode*)malloc(sizeof(bstnode));
//             newnode->data = data;
//             newnode->left = NULL;
//             newnode->right = NULL;
//             tnode->right = newnode;
//         } else {
//             bst_insert_helper(tnode->right, data);
//         }
//     }
// }


bstnode* bst_insert_helper(bstnode* root, int data) {
    if (root == NULL) {
        bstnode* newnode = (bstnode*)malloc(sizeof(bstnode));
        newnode->data = data;
        newnode->left = NULL;
        newnode->right = NULL;
        return newnode;
    }

    if (data > root->data) {
        root->right = bst_insert_helper(root->right, data);
    } else {
        root->left = bst_insert_helper(root->left, data);
    }

    return root;
}

bstnode* bst_find_helper(bstnode* tnode, int data) {
    if (tnode == NULL) {
        return NULL;
    }

    if (data > tnode->data) {
        return bst_find_helper(tnode->right, data);
    }
    else if (data < tnode->data) {
        return bst_find_helper(tnode->left, data);
    }
    else {
        return tnode;
    }
}

bstnode* bst_find(BST* bst, int data) {
    bstnode* result = bst_find_helper(bst->head, data);
    return result;
}


void bst_insert(BST* bst, int data){
    if (bst->head == NULL) { // If the tree is empty, create the root node
        bst->head = (bstnode*)malloc(sizeof(bstnode));
        bst->head->data = data;
        bst->head->left = NULL;
        bst->head->right = NULL;
    } else {
        bst_insert_helper(bst->head, data);
    }
    bst->size++;
}

void bst_print_helper(bstnode* tnode){
    if (tnode == NULL) {
        return;
    }
    bst_print_helper(tnode->left);
    printf("%d ", tnode->data);
    bst_print_helper(tnode->right);
}

void bst_print(BST* bst){
    bst_print_helper(bst->head);
    printf("\n");
}

#endif
