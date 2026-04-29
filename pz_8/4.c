#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();
    
    if (pid > 0) {
        printf("(Parent proc) Child ID: %d\n", pid);
        wait(NULL);
    } else if (pid == 0) {
        printf("(Child proc) PID: %d\n", pid);
    }
    
    return 0;
}
