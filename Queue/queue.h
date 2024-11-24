#ifndef VECTOR
#define VECTOR
#include "vector.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#ifndef QUEUE
#define QUEUE

typedef struct queue{
    vector* v;
    size_t front;
    size_t rear;
    size_t count;
}queue;

void q_init(queue* q){
    q->v = (vector*)malloc(sizeof(vector));
    vec_init(q->v);
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

int q_isempty(queue* q){
    return q->count == 0;
}

int q_isfull(queue* q){
    return (q->count == q->v->capacity);
}

void q_push_back(queue*q ,int val){
    q->count++;

    if(q_isfull(q)){
        vec_resize(q->v,q->v->capacity*2);
        if(q->front > q->rear){
            size_t newrear = q->v->capacity / 2;
            // there can be error bcuz q->front is given by chadgpt
            for(size_t i = 0; i<q->front; i++){
                q->v->data[newrear+i] = q->v->data[i];
            }

            q->rear = newrear + q->front - 1;
            q->front = newrear;
        }
    }

    q->v->data[q->rear] = val;
    q->rear = (q->rear + 1) % q->v->capacity;
    q->v->size++;
    // printf("%d",q->v->data[q->rear]);

    // printf("%d ",q->rear);
}



void q_push_front(queue* q,int val){
    q->count++;
    if(q_isfull(q)){
        vec_resize(q->v,q->v->capacity*2);
        if(q->front > q->rear){
            size_t newfront = q->v->capacity/2;
            for(size_t i= 0;i<q->count;i++){
                q->v->data[newfront+i] = q->v->data[(q->front+i)%q->v->capacity];
            }

            q->rear = newfront + q->count -1;
            q->front = newfront;
        }
    }

    q->front = (q->front -1 + q->v->capacity)%q->v->capacity;
    q->v->data[q->front] = val;
    q->v->size++;
}

int q_pop_front(queue* q){
    if(q_isempty(q)){
        printf("Queue underFlow\n");
    }

    int val = q->v->data[q->front];
    q->front = (q->front + 1)% q->v->capacity;
    q->v->size--;
    q->count--;
    return val;
}

int q_pop_back(queue* q){
    if(q_isempty(q)){
        printf("Queue UnderFlow\n");
    }

    q->rear = (q->rear - 1 + q->v->capacity)%q->v->capacity;
    int value = q->v->data[q->rear];
    q->v->size--;
    q->count--;

    return value;
}

int q_get_front(queue* q){
    return q->v->data[q->front];
}

int q_get_back(queue* q){
    return q->v->data[(q->rear - 1 + q->v->capacity)%q->v->capacity];
}

void q_print(queue *q) {
    if (q_isempty(q)) {
        printf("Queue is empty\n");
        return;
    }

    printf("Queue elements:\n");
    size_t index = q->front;

    for (size_t i = 0; i < q->count; i++) {
        printf("%d ", q->v->data[index]);
        index = (index + 1) % q->v->capacity;
    }
    printf("\n");
}

void q_free(queue* q) {
    if (q == NULL) return; 

    if (q->v != NULL) {
        vec_free(q->v); 
        free(q->v);     
        q->v = NULL;    
    }

    free(q); 
}


#endif