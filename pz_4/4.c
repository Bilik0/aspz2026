#include <stdio.h>
#include <stdlib.h>

void bad_code() {
    printf("--- Проблемний код ---\n");
    void *ptr = NULL;
    for (int i = 0; i < 2; i++) {
        if (!ptr) {
            ptr = malloc(10);
            printf("Allocated: %p\n", ptr);
        } else {
            printf("Помилка: ptr не NULL, але вказує на звільнену пам'ять: %p\n", ptr);
        }
        free(ptr);
    }
}

void good_code() {
    printf("\n--- Виправлений код ---\n");
    void *ptr = NULL;
    for (int i = 0; i < 2; i++) {
        if (!ptr) {
            ptr = malloc(10);
            printf("Allocated: %p\n", ptr);
        }
        free(ptr);
        ptr = NULL;
    }
}

int main() {
    bad_code();
    good_code();
    return 0;
}
