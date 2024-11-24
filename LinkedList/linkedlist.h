#ifndef LINKEDLIST
#define LINKEDLIST

#include "node.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct LL {
    node* head;
    node* tail;
    int size;
} LL;

void ll_init(LL* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void ll_insertatfront(LL* list, int data) {
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) return;

    list->size++;

    newNode->data = data;
    newNode->next = list->head;
    list->head = newNode;

    if (list->tail == NULL) {
        list->tail = newNode;
    }
}

void ll_insertatback(LL* list, int data) {
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) return;

    list->size++;
    
    newNode->data = data;
    newNode->next = NULL;

    if (list->tail == NULL) {  
        list->head = newNode;  
        list->tail = newNode;
    } else {
        list->tail->next = newNode;  
        list->tail = newNode;        
    }
}

void ll_deletebyvalue(LL* list, int val) {

    list->size--;
    node* current = list->head;
    node* prev = NULL;

    if (current != NULL && current->data == val) {
        list->head = current->next;
        free(current);
        return;
    }

    while (current != NULL && current->data != val) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) return;

    prev->next = current->next;
    free(current);
}

void ll_print(LL* list) {
    node* current = list->head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void ll_free(LL* list) {
    node* current = list->head;
    node* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    list->head = NULL;
}

#endif
