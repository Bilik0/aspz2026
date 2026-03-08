#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr = malloc(10);
    printf("Allocated 10 bytes at ptr: %p\n", ptr);

    void *ptr1 = realloc(ptr, (size_t)-1);

    if (!ptr1) {
        printf("Didn't realloc to ptr1 (returned NULL)\n");
        printf("Original ptr is still valid: %p\n", ptr);
        free(ptr);
    } else {
        free(ptr1);
    }
    return 0;
}

