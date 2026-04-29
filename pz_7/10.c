#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: Please provide a number 'n' as an argument.\n");
        printf("Example: %s 5.5\n", argv[0]);
        return 1;
    }

    double n = atof(argv[1]);
    srand(time(NULL) ^ getpid());

    double r1 = (double)rand() / RAND_MAX;
    printf("%f\n", r1);

    double r2 = ((double)rand() / RAND_MAX) * n;
    printf("%f\n", r2);

    return 0;
}
