#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>

void handler(int signum) {
    printf("%d\n", signum);
}

int main() {
    const char *fifo = "test_fifo";
    mkfifo(fifo, 0666);
    
    signal(SIGPIPE, handler);
    
    int fd = open(fifo, O_RDWR);
    int fd_read = open(fifo, O_RDONLY | O_NONBLOCK);
    close(fd_read);
    
    ssize_t ret = write(fd, "a", 1);
    if (ret == -1) {
        printf("%d\n", errno);
    }
    
    close(fd);
    unlink(fifo);
    return 0;
}
