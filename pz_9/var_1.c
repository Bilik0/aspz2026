#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void test_write(const char *filepath, const char *scenario) {
    FILE *f = fopen(filepath, "a");
    if (f) {
        fprintf(f, "Тестовий запис\n");
        fclose(f);
        printf("%s: Успішно змінено.\n", scenario);
    } else {
        printf("%s: Помилка доступу.\n", scenario);
    }
}

int main() {
    char cmd[256];

    system("sudo touch /tmp/scen1.txt");
    system("sudo chown root:$(id -gn) /tmp/scen1.txt");
    system("sudo chmod 664 /tmp/scen1.txt");
    test_write("/tmp/scen1.txt", "Сценарій 1 (Права групи)");

    system("sudo touch /tmp/scen2.txt");
    system("sudo chown root:root /tmp/scen2.txt");
    system("sudo chmod 666 /tmp/scen2.txt");
    test_write("/tmp/scen2.txt", "Сценарій 2 (Права для всіх)");

    system("sudo touch /tmp/scen3.txt");
    system("sudo chown root:root /tmp/scen3.txt");
    system("sudo chmod 644 /tmp/scen3.txt"); 
    
    snprintf(cmd, sizeof(cmd), "sudo setfacl -m u:%s:rw /tmp/scen3.txt", getenv("USER"));
    system(cmd);
    
    test_write("/tmp/scen3.txt", "Сценарій 3 (ACL)");

    system("sudo rm -f /tmp/scen1.txt /tmp/scen2.txt /tmp/scen3.txt");

    return 0;
}
