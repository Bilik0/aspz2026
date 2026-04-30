#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char path[256];
    char cmd[512];
    snprintf(path, sizeof(path), "%s/root_copy.txt", getenv("HOME"));

    system("touch test_file.txt");
    
    snprintf(cmd, sizeof(cmd), "sudo cp test_file.txt %s && sudo chown root:root %s", path, path);
    system(cmd);

    printf("Спроба запису у файл (власник root):\n");
    FILE *f = fopen(path, "a");
    if (f == NULL) {
        perror("Помилка запису (Permission denied)");
    } else {
        fprintf(f, "Текст\n");
        fclose(f);
    }

    printf("\nСпроба видалення файлу команди rm:\n");
    if (remove(path) == 0) {
        printf("Файл успішно видалено!\n");
    } else {
        perror("Помилка видалення");
    }

    return 0;
}
