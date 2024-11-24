#ifndef STACK
#define STACK

#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
    vector *v;
} stack;

void stack_init(stack *s) {
    s->v  = (vector*)malloc(sizeof(vector));
    vec_init(s->v);
}

void stack_push(stack *s, int value) {
    if (s->v->size == s->v->capacity) {
        vec_resize(s->v, s->v->capacity * 2);
    }
    s->v->data[s->v->size++] = value;
}

int stack_pop(stack *s) {
    if (s->v->size == 0) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return s->v->data[--s->v->size];
}

int stack_top(stack *s) {
    if (s->v->size == 0) {
        fprintf(stderr, "Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return s->v->data[s->v->size - 1];
}

size_t stack_size(stack *s) {
    return vec_size(s->v);
}

void stack_free(stack* s) {
    if (s == NULL) return; 

    if (s->v != NULL) {
        vec_free(s->v); 
        free(s->v);     
        s->v = NULL;    
    }

    free(s); 
}

#endif