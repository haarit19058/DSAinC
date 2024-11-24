#ifndef NODE
#define NODE

typedef struct node {
    int data;
    int key;
    struct node* next;
} node;

typedef struct bstnode{
    int data;
    struct bstnode* left;
    struct bstnode* right;
}bstnode;

#endif