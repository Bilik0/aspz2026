#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("--- Вміст домашнього каталогу ---\n");
    system("ls -l ~ | head -n 5");
    
    printf("\n--- Вміст /usr/bin ---\n");
    system("ls -l /usr/bin | head -n 5");
    
    printf("\n--- Вміст /etc ---\n");
    system("ls -l /etc | head -n 5");

    printf("\n--- Спроба читання /etc/shadow ---\n");
    FILE *f_read = fopen("/etc/shadow", "r");
    if (!f_read) perror("Читання заблоковано");
    else fclose(f_read);

    printf("\n--- Спроба запису в /etc/config_test.txt ---\n");
    FILE *f_write = fopen("/etc/config_test.txt", "w");
    if (!f_write) perror("Запис заблоковано");
    else { 
        fclose(f_write); 
        remove("/etc/config_test.txt"); 
    }

    printf("\n--- Спроба виконання /usr/bin/passwd ---\n");
    if (access("/usr/bin/passwd", X_OK) == 0) {
        printf("Виконання дозволено (але сама дія вимагає додаткових привілеїв).\n");
    } else {
        perror("Виконання заблоковано");
    }

    return 0;
}
