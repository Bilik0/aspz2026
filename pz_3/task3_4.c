#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void signal_handler(int signum) {
    if (signum == SIGXCPU) {
        fprintf(stderr, "\n[Помилка] Ліміт ресурсу часу ЦП вичерпано (SIGXCPU)!\n");
        exit(1);
    }
}

void print_lottery(int count, int max_val) {
    int *numbers = (int*)malloc(max_val * sizeof(int));
    for (int i = 0; i < max_val; ++i) {
        numbers[i] = i + 1;
    }
    
    for (int i = max_val - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }
    
    for (int i = 0; i < count; ++i) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    free(numbers);
}

int main() {
    signal(SIGXCPU, signal_handler);
    srand(time(NULL));

    struct rlimit rl;
    rl.rlim_cur = 1; 
    rl.rlim_max = 1;
    setrlimit(RLIMIT_CPU, &rl);

    while (1) {
        printf("7 з 49: ");
        print_lottery(7, 49);
        printf("6 з 36: ");
        print_lottery(6, 36);
    }
    return 0;
}
