#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    struct passwd *pw;
    uid_t current_uid = getuid();

    setpwent(); 
    printf("Звичайні користувачі в системі (UID >= 1000):\n");
    
    while ((pw = getpwent()) != NULL) {
        if (pw->pw_uid >= 1000 && pw->pw_uid != current_uid && pw->pw_uid != 65534) {
            printf("%s (UID: %d)\n", pw->pw_name, pw->pw_uid);
        }
    }
    
    endpwent(); 
    return 0;
}
