#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    if (signum == SIGXFSZ) {
        fprintf(stderr, "\n[Помилка] Перевищено ліміт розміру файлу (SIGXFSZ)! Завершення роботи.\n");
        exit(1);
    }
}

int main() {
    signal(SIGXFSZ, signal_handler);

    struct rlimit rl;
    rl.rlim_cur = 100;
    rl.rlim_max = 100;
    setrlimit(RLIMIT_FSIZE, &rl);

    FILE *file = fopen("dice_rolls.txt", "w");
    if (!file) return 1;

    while (1) {
        int roll = rand() % 6 + 1;
        fprintf(file, "Кидок: %d\n", roll);
        fflush(file);
        printf("Записано: %d\n", roll);
    }
    return 0;
}
