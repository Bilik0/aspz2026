#include <stdio.h>
#include <stdlib.h>

int main() {
    size_t size = 1024 * 1024 * 100;
    size_t total = 0;
    void *ptr = NULL;

    while (size > 0) {
        ptr = malloc(size);
        if (ptr != NULL) {
            total += size;
        } else {
            size /= 2;
        }
    }

    printf("Total allocated heap: %zu MB\n", total / (1024 * 1024));
    return 0;
}
