#include <stdio.h>
#include <stdlib.h>

void countingSort(int* arr, int size, int k) {
    int* temp = (int*)malloc(sizeof(int) * k);

    // Initialize the temporary array to 0
    for (int i = 0; i < k; i++) {
        temp[i] = 0;
    }

    // Count the occurrences of each number
    for (int i = 0; i < size; i++) {
        temp[arr[i]]++;
    }

    // Sort the array based on counts
    int index = 0;
    for (int i = 0; i < k; i++) {
        while (temp[i] > 0) {
            arr[index++] = i;
            temp[i]--;
        }
    }

    free(temp);
}

void fractionalCountingSort(double* arr, int size, int k, int d) {
    int scale = (int)pow(10, d);  
    int range = scale * k;        

    int* count = (int*)calloc(range + 1, sizeof(int));

    int* scaledArr = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        scaledArr[i] = (int)(arr[i] * scale);  
        count[scaledArr[i]]++;
    }

    int index = 0;
    for (int i = 0; i <= range; i++) {
        while (count[i] > 0) {
            arr[index++] = (double)i / scale;
            count[i]--;
        }
    }

    free(count);
    free(scaledArr);
}

void countSortRad(int* arr, int n, int exp) {
    int output[n];
    int i, count[10] = {0};

    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixsort(int* arr, int n) {
    int m = 8;
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSortRad(arr, n, exp);
}

void insertionSort(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void bucketSort(float* arr, int n) {
    int i;
    float minVal = arr[0], maxVal = arr[0];

    for (i = 1; i < n; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
    }

    int bucketCount = n;
    int **buckets = (int**)malloc(bucketCount * sizeof(int*));
    int* bucketSizes = (int*)malloc(bucketCount * sizeof(int));
    for (i = 0; i < bucketCount; i++) {
        buckets[i] = (int*)malloc(sizeof(int) * n);
        bucketSizes[i] = 0; 
    }

    for (i = 0; i < n; i++) {
        int bucketIndex = (arr[i] - minVal) * bucketCount / (maxVal - minVal + 1e-9);
        buckets[bucketIndex][bucketSizes[bucketIndex]++] = arr[i];
    }

    for (i = 0; i < bucketCount; i++) {
        if (bucketSizes[i] > 0) {
            insertionSort(buckets[i], bucketSizes[i]);
        }
    }

    int index = 0;
    for (i = 0; i < bucketCount; i++) {
        for (int j = 0; j < bucketSizes[i]; j++) {
            arr[index++] = buckets[i][j];
        }
    }

    for (i = 0; i < bucketCount; i++) {
        free(buckets[i]);
    }
    free(buckets);
    free(bucketSizes);
}



int main() {
    srand(200);

    int size = 10000000;   
    int k = 2000000;      

    
    int* arr = (int*)malloc(sizeof(int) * size);

    
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % k;
    }

    
    // printf("Original array:\n");
    // for (int i = 0; i < size; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    
    countingSort(arr, size, k);

    
    // printf("Sorted array:\n");
    // for (int i = 0; i < size; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    free(arr);

    return 0;
}
