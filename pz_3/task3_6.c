#include <stdio.h>
#include <sys/resource.h>

void deep_recursion(int depth) {
    char buffer[1024];
    printf("Глибина рекурсії: %d\n", depth);
    deep_recursion(depth + 1);
}

int main() {
    struct rlimit rl;
    rl.rlim_cur = 1024 * 1024;
    rl.rlim_max = 1024 * 1024;
    setrlimit(RLIMIT_STACK, &rl);

    printf("Запуск рекурсії з лімітом стека 1 МБ...\n");
    deep_recursion(1);

    return 0;
}
