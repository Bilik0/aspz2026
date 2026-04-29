#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_permissions(mode_t mode) {
    printf("%c", S_ISDIR(mode) ? 'd' : '-');
    printf("%c", mode & S_IRUSR ? 'r' : '-');
    printf("%c", mode & S_IWUSR ? 'w' : '-');
    printf("%c", mode & S_IXUSR ? 'x' : '-');
    printf("%c", mode & S_IRGRP ? 'r' : '-');
    printf("%c", mode & S_IWGRP ? 'w' : '-');
    printf("%c", mode & S_IXGRP ? 'x' : '-');
    printf("%c", mode & S_IROTH ? 'r' : '-');
    printf("%c", mode & S_IWOTH ? 'w' : '-');
    printf("%c", mode & S_IXOTH ? 'x' : '-');
}

int main() {
    DIR *d;
    struct dirent *dir;
    struct stat fileStat;
    struct passwd *pw;
    struct group *gr;
    char time_buf[80];

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (stat(dir->d_name, &fileStat) < 0) continue;
            print_permissions(fileStat.st_mode);
            printf(" %lu", (unsigned long)fileStat.st_nlink);
            pw = getpwuid(fileStat.st_uid);
            printf(" %s", pw ? pw->pw_name : "unknown");
            gr = getgrgid(fileStat.st_gid);
            printf(" %s", gr ? gr->gr_name : "unknown");
            printf(" %5ld", (long)fileStat.st_size);
            strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", localtime(&fileStat.st_mtime));
            printf(" %s %s\n", time_buf, dir->d_name);
        }
        closedir(d);
    }
    return 0;
}
