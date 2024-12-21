#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArr(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Vanilla Quick Sort

int partition(int* arr, int start, int end) {
    int pivot = arr[start];
    int i = start + 1;
    int j = end;

    while (i <= j) {
        while (i <= j && arr[i] < pivot) {
            i++;
        }
        while (i <= j && arr[j] >= pivot) {
            j--;
        }
        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[start], &arr[j]); // Place pivot in the correct position
    return j;
}

void quicksort(int* arr, int start, int end) {
    if (start >= end) return;

    int index = partition(arr, start, end);
    quicksort(arr, start, index - 1);
    quicksort(arr, index + 1, end);
}

// Randomized Quick Sort

int randomizedPartition(int* arr, int start, int end) {
    int randomIndex = start + rand() % (end - start + 1);
    swap(&arr[start], &arr[randomIndex]); // Place random pivot at the start
    int pivot = arr[start];
    int i = start + 1;
    int j = end;

    while (i <= j) {
        while (i <= j && arr[i] < pivot) {
            i++;
        }
        while (i <= j && arr[j] >= pivot) {
            j--;
        }
        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[start], &arr[j]); // Place pivot in correct position
    return j;
}

void randomizedQuicksort(int* arr, int start, int end) {
    if (start >= end) return;

    int index = randomizedPartition(arr, start, end);
    quicksort(arr, start, index - 1);
    quicksort(arr, index + 1, end);
}


// Three partition quick sort

void threewaypartition(int *arr, int p, int r, int *q1, int *q2) {
    int x = arr[r];
    int i = p - 1;
    int j = p;
    int k = r;

    while (j <= k) {
        if (arr[j] < x) {
            swap(&arr[i + 1], &arr[j]);
            i++;
            j++;
        } else if (arr[j] > x) {
            swap(&arr[j], &arr[k]);
            k--;
        } else {
            j++;
        }
    }

    *q1 = i + 1;
    *q2 = k;
}

void quicksortthwp(int *arr, int p, int r) {
    if (p >= r) return;

    int q1, q2;
    threewaypartition(arr, p, r, &q1, &q2);

    quicksortthwp(arr, p, q1 - 1); // Left part
    quicksortthwp(arr, q2 + 1, r); // Right part
}


void demosort(int* arr,int size,int start,int end){
    // printArr(arr,size);
    if(arr[start] > arr[end]){
        swap(arr+start,arr+end);
    }

    if(start+1<end){
        int k = (end-start+1)/3;
        demosort(arr,size,start,end-k);
        demosort(arr,size,start+k,end);
        demosort(arr,size,start,end-k);
    }
}


void  trequicksort(int* arr,int start,int end){
    while(start<end){
        int index = partition(arr,start,end);
        trequicksort(arr,start,end-1);
        start = index+1;
    }
}

void optimized_tre_quicksort(int* A, int p, int r) {
    while (p < r) {
        int q = partition(A, p, r);

        // Always recurse on the smaller side to limit stack depth
        if (q - p < r - q) {
            optimized_tre_quicksort(A, p, q - 1); // Sort left part recursively
            p = q + 1; // Sort the right part iteratively
        } else {
            optimized_tre_quicksort(A, q + 1, r); // Sort right part recursively
            r = q - 1; // Sort the left part iteratively
        }
    }
}

int main() {
    int size = 10;
    int* arr = (int*)malloc(sizeof(int) * size);

    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % size;
    }

    printf("Original array:\n");
    printArr(arr, size);

    // randomizedQuicksort(arr, 0, size - 1);
    trequicksort(arr,0,size-1);

    printf("Sorted array:\n");
    printArr(arr, size);

    free(arr); // Clean up memory
    return 0;
}
