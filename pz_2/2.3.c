#include <stdio.h>
#include <stdlib.h>

int data_var = 50;
int bss_var;

int main() {
    int stack_var;
    void *heap_ptr = malloc(1024);

    printf("Text segment (code) near: %p\n", (void*)main);
    printf("Data segment near: %p\n", (void*)&data_var);
    printf("BSS segment near: %p\n", (void*)&bss_var);
    printf("Heap location: %p\n", heap_ptr);
    printf("Stack top near: %p\n", (void*)&stack_var);

    free(heap_ptr);
    return 0;
}
