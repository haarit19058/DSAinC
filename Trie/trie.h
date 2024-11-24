#ifndef TRIE
#define TRIE

#define SIZE 26

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct trienode {
    struct trienode* child[SIZE];
    bool isEnd;
} trienode;

typedef struct trie {
    trienode* head;
    int size;
} trie;

trienode* create_node() {
    trienode* node = (trienode*)malloc(sizeof(trienode));
    for (int i = 0; i < SIZE; i++) {
        node->child[i] = NULL;
    }
    node->isEnd = false;
    return node;
}

void trie_init(trie* trie) {
    trie->head = create_node();
    trie->size = 0;
}

void trie_insert(trie* trie, const char* word) {
    trienode* temp = trie->head;
    while (*word) {
        int index = *word - 'a';
        if (!temp->child[index]) {
            temp->child[index] = create_node();
        }
        temp = temp->child[index];
        word++;
    }
    if (!temp->isEnd) {
        temp->isEnd = true;
        trie->size++;
    }
}

bool trie_search(trie* trie, const char* word) {
    trienode* temp = trie->head;
    while (*word) {
        int index = *word - 'a';
        if (!temp->child[index]) {
            return false;
        }
        temp = temp->child[index];
        word++;
    }
    return temp != NULL && temp->isEnd;
}

bool trie_starts_with(trie* trie, const char* prefix) {
    trienode* temp = trie->head;
    while (*prefix) {
        int index = *prefix - 'a';
        if (!temp->child[index]) {
            return false;
        }
        temp = temp->child[index];
        prefix++;
    }
    return true;
}

void trie_free_node(trienode* node) {
    for (int i = 0; i < SIZE; i++) {
        if (node->child[i]) {
            trie_free_node(node->child[i]);
        }
    }
    free(node);
}

void trie_free(trie* trie) {
    if (trie->head) {
        trie_free_node(trie->head);
    }
}

#endif
