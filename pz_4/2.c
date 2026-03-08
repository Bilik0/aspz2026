#include <stdio.h>
#include <stdlib.h>

int main() {
    int xa = 2147483647;
    int xb = 2;
    int num = xa * xb;

    printf("xa: %d, xb: %d\n", xa, xb);
    printf("num (signed int): %d\n", num);

    size_t num_sizet = (size_t)num;
    printf("num_sizet (unsigned): %zu\n", num_sizet);

    void *ptr = malloc(num);
    if (ptr) {
        printf("Allocated successfully\n");
        free(ptr);
    } else {
        printf("Didn't allocate %zu bytes\n", num_sizet);
    }
    return 0;
}
