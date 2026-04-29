#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

int main() {
    DIR *d;
    struct dirent *dir;
    char response;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            printf("%s? (y/n): ", dir->d_name);
            scanf(" %c", &response);
            if (response == 'y' || response == 'Y') {
                unlink(dir->d_name);
            }
        }
        closedir(d);
    }
    return 0;
}
