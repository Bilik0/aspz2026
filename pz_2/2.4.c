#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void bar_is_now_closed() {
    int local = 1;
    printf("Inside bar_is_now_closed, local at: %p\n", (void*)&local);
    pause();
}

void bar() {
    int local = 2;
    printf("Inside bar, local at: %p\n", (void*)&local);
    bar_is_now_closed();
}

void foo() {
    int local = 3;
    printf("Inside foo, local at: %p\n", (void*)&local);
    bar();
}

int main() {
    int local = 4;
    printf("Inside main, local at: %p\n", (void*)&local);
    foo();
    return 0;
}
