#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    if (signum == SIGXFSZ) {
        fprintf(stderr, "\n[Помилка] Перевищено ліміт на розмір вихідного файлу!\n");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Program need two arguments\n");
        return 1;
    }

    if (access(argv[1], R_OK) != 0) {
        fprintf(stderr, "Cannot open file %s for reading\n", argv[1]);
        return 1;
    }

    signal(SIGXFSZ, signal_handler);
    
    struct rlimit rl;
    rl.rlim_cur = 50; 
    rl.rlim_max = 50;
    setrlimit(RLIMIT_FSIZE, &rl);

    FILE *src = fopen(argv[1], "rb");
    FILE *dst = fopen(argv[2], "wb");

    if (!dst) {
        fprintf(stderr, "Cannot open file %s for writing\n", argv[2]);
        if (src) fclose(src);
        return 1;
    }

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
    }

    printf("Файл успішно скопійовано.\n");
    fclose(src);
    fclose(dst);
    return 0;
}
