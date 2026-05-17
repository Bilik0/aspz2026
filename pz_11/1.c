#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>
#include <fcntl.h>
#include <string.h>

char **global_argv;

void int_to_str(int val, char *buf, int *len) {
    char temp[32];
    int i = 0;
    if (val == 0) {
        temp[i++] = '0';
    } else {
        while (val > 0) {
            temp[i++] = (val % 10) + '0';
            val /= 10;
        }
    }
    *len = 0;
    while (i > 0) {
        buf[(*len)++] = temp[--i];
    }
    buf[*len] = '\0';
}

void ptr_to_hex_str(void *ptr, char *buf, int *len) {
    unsigned long val = (unsigned long)ptr;
    char temp[32];
    int i = 0;
    if (val == 0) {
        temp[i++] = '0';
    } else {
        while (val > 0) {
            int rem = val % 16;
            temp[i++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'a');
            val /= 16;
        }
    }
    *len = 0;
    buf[(*len)++] = '0';
    buf[(*len)++] = 'x';
    while (i > 0) {
        buf[(*len)++] = temp[--i];
    }
    buf[*len] = '\0';
}

void sigsegv_handler(int sig, siginfo_t *info, void *ucontext) {
    int fd = open("crash_log.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd != -1) {
        char buf[256];
        int len;

        write(fd, "--- CRASH DETECTED ---\n", 23);

        write(fd, "PID: ", 5);
        int_to_str(getpid(), buf, &len);
        write(fd, buf, len);
        write(fd, "\n", 1);

        write(fd, "Fault Address: ", 15);
        ptr_to_hex_str(info->si_addr, buf, &len);
        write(fd, buf, len);
        write(fd, "\n", 1);

        write(fd, "Stack Trace:\n", 13);
        void *array[20];
        size_t size = backtrace(array, 20);
        backtrace_symbols_fd(array, size, fd);

        write(fd, "Restarting application...\n\n", 27);
        close(fd);
    }

    execv("/proc/self/exe", global_argv);
    _exit(1); 
}

void trigger_segmentation_fault() {
    printf("Генеруємо SIGSEGV...\n");
    int *ptr = NULL;
    *ptr = 42;
}

int main(int argc, char *argv[]) {
    global_argv = argv;

    printf("Програма запущена. PID: %d\n", getpid());

    if (argc > 1 && strcmp(argv[1], "recover") == 0) {
        printf("Програму успішно відновлено після збою!\n");
        return 0; 
    }

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO | SA_NODEFER;
    sa.sa_sigaction = sigsegv_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("Не вдалося встановити обробник sigaction");
        exit(EXIT_FAILURE);
    }

    sleep(2); 

    char *new_argv[] = {argv[0], "recover", NULL};
    global_argv = new_argv;

    trigger_segmentation_fault();

    return 0;
}
