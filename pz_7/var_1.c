#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_PATH 4096
#define MAX_VISITED 10000

typedef struct {
    dev_t dev;
    ino_t ino;
} VisitedNode;

VisitedNode visited[MAX_VISITED];
int visited_count = 0;

int is_visited(dev_t dev, ino_t ino) {
    for (int i = 0; i < visited_count; i++) {
        if (visited[i].dev == dev && visited[i].ino == ino) {
            return 1;
        }
    }
    return 0;
}

void add_visited(dev_t dev, ino_t ino) {
    if (visited_count < MAX_VISITED) {
        visited[visited_count].dev = dev;
        visited[visited_count].ino = ino;
        visited_count++;
    }
}

void print_hierarchy(const char *path, int level) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[MAX_PATH];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat lstatbuf;
        if (lstat(full_path, &lstatbuf) == -1) {
            continue;
        }

        if (S_ISDIR(lstatbuf.st_mode) || S_ISLNK(lstatbuf.st_mode)) {
            struct stat target_stat;
            int is_dir = 0;
            int stat_res = stat(full_path, &target_stat);

            if (stat_res == 0 && S_ISDIR(target_stat.st_mode)) {
                is_dir = 1;
            }

            if (S_ISDIR(lstatbuf.st_mode)) {
                is_dir = 1;
            }

            if (!is_dir && !S_ISLNK(lstatbuf.st_mode)) {
                continue;
            }

            if (stat_res == 0) {
                if (is_visited(target_stat.st_dev, target_stat.st_ino)) {
                    continue;
                }
                add_visited(target_stat.st_dev, target_stat.st_ino);
            }

            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("|-- %s\n", entry->d_name);

            if (is_dir && S_ISDIR(lstatbuf.st_mode)) {
                print_hierarchy(full_path, level + 1);
            }
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {
    const char *start_path = getenv("HOME");
    if (start_path == NULL) {
        start_path = "/";
    }

    if (argc > 1) {
        start_path = argv[1];
    }

    struct stat statbuf;
    if (stat(start_path, &statbuf) != -1) {
        add_visited(statbuf.st_dev, statbuf.st_ino);
    }

    printf("%s\n", start_path);
    print_hierarchy(start_path, 1);

    return 0;
}
