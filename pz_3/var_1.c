#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* thread_func(void* arg) {
    sleep(60); 
    return NULL;
}

int main() {
    int count = 0;
    printf("Починаємо створення потоків...\n");

    while (1) {
        pthread_t t;
        int result = pthread_create(&t, NULL, thread_func, NULL);
        
        if (result != 0) {
            fprintf(stderr, "\n[Помилка] Не вдалося створити потік! Досягнуто ліміт.\n");
            fprintf(stderr, "Всього створено потоків: %d\n", count);
            break;
        }
        
        count++;
        
        if (count % 50 == 0) {
            printf("Створено %d потоків...\n", count);
        }
    }

    return 0;
}
