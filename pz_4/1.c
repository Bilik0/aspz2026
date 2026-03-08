#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main() {
    size_t max_size = (size_t)-1;
    printf("Bits in size_t: %zu\n", sizeof(size_t) * 8);
    printf("Max size_t (bytes): %zu\n", max_size);
    printf("Theoretical max allocation: %llu EB (Exabytes)\n", 
           max_size / (1024ULL * 1024 * 1024 * 1024 * 1024 * 1024));
           
    return 0;
}
