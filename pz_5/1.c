#include <stdio.h>

int main() {
    int admin_privilege = 0; 
    int arr[3] = {10, 20, 30};

    printf("Початковий статус адміністратора: %d\n", admin_privilege);
    printf("Елементи масиву: %d, %d, %d\n", arr[0], arr[1], arr[2]);

    int leaked_value = arr[3]; 
    
    printf("\n[!] Зчитуємо arr[3] (Out-of-bounds access)...\n");
    printf("Значення прочитане за межами масиву: %d\n", leaked_value);

    if (leaked_value == 0) {
        printf("\nЛогіку змінено: Система надала адмінські права!\n");
        admin_privilege = 1; 
    }

    printf("Кінцевий статус адміністратора: %d\n", admin_privilege);

    return 0;
}
