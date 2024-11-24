#ifndef HEAP
#define HEAP
#include "vector.h"

typedef struct maxHeap {
    vector* data;
} maxHeap;

void mh_init(maxHeap* heap) {
    heap->data = (vector*)malloc(sizeof(vector));
    vec_init(heap->data);
}

void mh_heapify(vector* data, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && vec_at(data, left) > vec_at(data, largest)) {
        largest = left;
    }

    if (right < n && vec_at(data, right) > vec_at(data, largest)) {
        largest = right;
    }

    if (largest != i) {
        int temp = vec_at(data, i);
        vec_change(data, i, vec_at(data, largest));
        vec_change(data, largest, temp);

        mh_heapify(data, n, largest);
    }
}

void mh_insert(maxHeap* heap, int value) {
    vec_push_back(heap->data, value);
    int i = vec_size(heap->data) - 1;

    while (i != 0 && vec_at(heap->data, (i - 1) / 2) < vec_at(heap->data, i)) {
        int parent = (i - 1) / 2;
        int temp = vec_at(heap->data, i);
        vec_change(heap->data, i, vec_at(heap->data, parent));
        vec_change(heap->data, parent, temp);
        i = parent;
    }
}

int mh_extract_max(maxHeap* heap) {
    if (vec_size(heap->data) == 0) {
        printf("Heap Underflow");
        return -1;
    }

    int max = vec_at(heap->data, 0);
    vec_change(heap->data, 0, vec_at(heap->data, vec_size(heap->data) - 1));
    vec_pop_back(heap->data);
    mh_heapify(heap->data, vec_size(heap->data), 0);

    return max;
}

void mh_free(maxHeap* heap) {
    vec_free(heap->data);
    free(heap->data);
}

void mh_print(maxHeap* heap){
    vec_print(heap->data);
}

#endif
