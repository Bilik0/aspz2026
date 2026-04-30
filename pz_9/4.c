#include <stdlib.h>
#include <stdio.h>

int main() {
    printf("--- Вивід команди whoami ---\n");
    system("whoami");
    
    printf("\n--- Вивід команди id ---\n");
    system("id");
    
    return 0;
}
