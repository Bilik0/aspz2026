#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void measure(int n, const char* label) {
    int *arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }
    
    clock_t start = clock();
    qsort(arr, n, sizeof(int), cmp);
    clock_t end = clock();
    
    printf("%s (n=%d): %.3f sec\n", label, n, (double)(end - start) / CLOCKS_PER_SEC);
    free(arr);
}

int main() {
    measure(100000, "Small");
    measure(1000000, "Large");
    return 0;
}
