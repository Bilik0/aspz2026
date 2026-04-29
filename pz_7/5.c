#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void list_files(const char *path) {
    DIR *dp = opendir(path);
    if (!dp) return;
    struct dirent *entry;
    struct stat statbuf;
    char full_path[1024];

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        printf("%s\n", full_path);
        if (stat(full_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            list_files(full_path);
        }
    }
    closedir(dp);
}

int main() {
    list_files(".");
    return 0;
}
