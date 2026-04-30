#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_access(const char *filepath) {
    if (access(filepath, R_OK) == 0) printf("  Читання: Дозволено\n");
    else printf("  Читання: Заборонено\n");
    
    if (access(filepath, W_OK) == 0) printf("  Запис: Дозволено\n");
    else printf("  Запис: Заборонено\n");
}

int main() {
    char template[] = "/tmp/mytempXXXXXX";
    int fd = mkstemp(template); 
    if (fd == -1) return 1;
    close(fd);

    char cmd[256];

    printf("--- Права 600, власник root ---\n");
    snprintf(cmd, sizeof(cmd), "sudo chown root:root %s && sudo chmod 600 %s", template, template);
    system(cmd);
    check_access(template);

    printf("\n--- Права 644, власник root ---\n");
    snprintf(cmd, sizeof(cmd), "sudo chmod 644 %s", template);
    system(cmd);
    check_access(template);

    printf("\n--- Права 666, власник root ---\n");
    snprintf(cmd, sizeof(cmd), "sudo chmod 666 %s", template);
    system(cmd);
    check_access(template);

    remove(template); 
    return 0;
}

