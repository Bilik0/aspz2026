#include <stdio.h>
#include <time.h>

int main() {
    printf("Size of time_t: %zu bytes\n", sizeof(time_t));

    time_t max_32bit = 2147483647;
    printf("Max 32-bit time: %s", ctime(&max_32bit));

    max_32bit++;
    printf("After overflow: %s", ctime(&max_32bit));

    return 0;
}
