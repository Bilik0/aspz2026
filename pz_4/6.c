#include <stdio.h>
#include <stdlib.h>

int main() {
    void *ptr1 = realloc(NULL, 10);
    printf("Reallocated ptr with NULL: %p\n", ptr1);

    void *ptr2 = realloc(ptr1, 0);
    if (!ptr2) {
        printf("realloc with 0 size returned NULL (memory freed)\n");
    }

    return 0;
}
