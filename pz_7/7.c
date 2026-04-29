#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    DIR *d;
    struct dirent *dir;
    char response;
    struct stat statbuf;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            char *ext = strrchr(dir->d_name, '.');
            if (ext && strcmp(ext, ".c") == 0) {
                printf("%s? (y/n): ", dir->d_name);
                scanf(" %c", &response);
                if (response == 'y' || response == 'Y') {
                    if (stat(dir->d_name, &statbuf) == 0) {
                        chmod(dir->d_name, statbuf.st_mode | S_IROTH);
                    }
                }
            }
        }
        closedir(d);
    }
    return 0;
}
