#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd[2];
    pipe(fd);
    fcntl(fd[1], F_SETFL, O_NONBLOCK);
    
    int req = 100000;
    char *buffer = malloc(req);
    ssize_t res = write(fd[1], buffer, req);
    
    printf("Requested: %d, Written: %zd\n", req, res);
    
    free(buffer);
    close(fd[0]);
    close(fd[1]);
    return 0;
}
