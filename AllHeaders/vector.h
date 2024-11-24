#ifndef VECTOR
#define VECTOR

#include <stdio.h>
#include <stdlib.h>

typedef struct vector{
    int *data;
    size_t size;
    size_t capacity;
}vector;

void vec_init(vector* v){
    v->size = 0;
    v->capacity = 100;
    v->data = (int*)malloc(v->capacity * sizeof(int));
}


// Function definitions
size_t vec_size(vector *v) {
    if (v == NULL) {
        fprintf(stderr, "Vector pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    // size_t temp = v->size;
    // printf("%d\n",v->size);
    return v->size;
}

size_t vec_capacity(vector *v) {
    if (v == NULL) {
        fprintf(stderr, "Vector pointer is NULL\n");
        exit(EXIT_FAILURE);
    }
    // size_t temp = v->capacity;
    return v->capacity;
}

int vec_at(vector* v,size_t index){
    if(index>=v->size){
        printf("Index out of bound\n");
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

void vec_change(vector* v,size_t index,int val){
    v->data[index] = val;
}

void vec_resize(vector* v,size_t new_capacity){
    v->capacity  = new_capacity;
    v->data = (int*) realloc(v->data,v->capacity*sizeof(int));
}

void vec_push_back(vector* v, int value){
    if(v->size == v->capacity){
        vec_resize(v,v->capacity*2);
    }
    v->data[v->size++] = value;
}

void vec_pop_back(vector* v){
    if(v->size>0){
        v->size--;
    }
    else{
        printf("vector is empty. Underflow\n");
    }
}

void vec_insert(vector *v, size_t index, int value) {
    if (index > v->size) {
        printf("Index out of bound\n");
        exit(EXIT_FAILURE);
    }
    if (v->size == v->capacity) {
        vec_resize(v, v->capacity * 2);
    }
    for (size_t i = v->size; i > index; i--) {
        v->data[i] = v->data[i - 1];
    }
    v->data[index] = value;
    v->size++;
}

void vec_erase(vector *v, size_t index) {
    if (index >= v->size) {
        printf("Index out of bound\n");
        exit(EXIT_FAILURE);
    }
    for (size_t i = index; i < v->size - 1; i++) {
        v->data[i] = v->data[i + 1];
    }
    v->size--;
}


void vec_print(vector* v){
    printf("[");
    for(size_t i = 0;i < v->size;i++){
        printf(" %d",v->data[i]);
        if (i < v->size - 1) {
                printf(", ");
            }
    }
    printf("]\n");
}

void vec_free(vector* v) {
    if (v->data != NULL) { // Check if data is already freed
        free(v->data);
        v->data = NULL; // Optional: Set the pointer to NULL to avoid dangling pointers
    }
    v->size = 0;
    v->capacity = 0;
}

#endif