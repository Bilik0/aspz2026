#include <stdio.h>

int main(int argc, char *argv[]) {
    int line_count = 0;
    char buffer[1024];

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            continue;
        }

        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
            line_count++;

            if (line_count % 20 == 0) {
                FILE *tty = fopen("/dev/tty", "r");
                if (tty != NULL) {
                    fgetc(tty);
                    fclose(tty);
                } else {
                    getchar();
                }
            }
        }
        fclose(fp);
    }
    return 0;
}
