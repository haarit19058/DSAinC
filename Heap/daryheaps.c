#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArr(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

typedef struct dheap {
    int* data;
    int nchild;
    int size;
} dheap;

void init(dheap* dh, int d) {
    dh->data = (int*)malloc(sizeof(int) * 200);
    dh->nchild = d;
    dh->size = 0;
}

void dheapify(dheap* dh, int index) {
    int d = dh->nchild;
    int largestIndex = index;

    for (int i = 1; i <= d; i++) {
        int childIndex = d * index + i;
        if (childIndex < dh->size && dh->data[childIndex] > dh->data[largestIndex]) {
            largestIndex = childIndex;
        }
    }

    if (largestIndex != index) {
        swap(&(dh->data[index]), &(dh->data[largestIndex]));
        dheapify(dh, largestIndex);
    }
}

void dbuildheap(dheap* dh) {
    for (int i = dh->size / dh->nchild - 1; i >= 0; i--) {
        dheapify(dh, i);
    }
}

void insert(dheap* dh, int data) {
    dh->size++;
    dh->data[dh->size - 1] = data;

    int index = dh->size - 1;
    while (index > 0 && dh->data[index] > dh->data[(index - 1) / dh->nchild]) {
        swap(&dh->data[index], &dh->data[(index - 1) / dh->nchild]);
        index = (index - 1) / dh->nchild;
    }
}

int main() {
    dheap* dh = (dheap*)malloc(sizeof(dheap));
    init(dh, 10);	

    srand(time(NULL)); 
    for (int i = 0; i < 100; i++) {
        int data = rand() % 100;
        insert(dh, data);
    }

    printArr(dh->data, dh->size);

    free(dh->data); 
    free(dh);

    return 0;
}
