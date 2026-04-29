#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main() {
    DIR *d;
    struct dirent *dir;
    struct stat statbuf;
    char *dirs[1000];
    int count = 0;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            if (stat(dir->d_name, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
                dirs[count] = strdup(dir->d_name);
                count++;
            }
        }
        closedir(d);
    }

    qsort(dirs, count, sizeof(char *), compare);

    for (int i = 0; i < count; i++) {
        printf("%s\n", dirs[i]);
        free(dirs[i]);
    }

    return 0;
}
