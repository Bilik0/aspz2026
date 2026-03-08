#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

struct sbar {
    int x;
    double y;
};

int main() {
    struct sbar *ptr, *newptr;

    ptr = calloc(1000, sizeof(struct sbar));
    printf("Callocated ptr: %p\n", (void*)ptr);

    newptr = reallocarray(ptr, 500, sizeof(struct sbar));
    printf("Reallocarray'd newptr: %p\n", (void*)newptr);

    if (newptr) {
        free(newptr);
    } else {
        free(ptr);
    }

    return 0;
}

