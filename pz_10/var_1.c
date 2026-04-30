#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_var = 10;

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        return 1;
    } else if (pid == 0) {
        sleep(1);
        printf("Child process: global_var = %d\n", global_var);
    } else {
        global_var = 20;
        printf("Parent process: global_var = %d\n", global_var);
        wait(NULL);
    }

    return 0;
}
