#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    FILE *fp;
    char path[1035];
    uid_t my_uid = getuid();

    fp = popen("getent passwd", "r");
    if(fp == NULL){
        printf("Не вдалося виконати команду getent passwd\n");
        return 1;
    }

    printf("Звичайні користувачі, окрім вашого (UID: %d):\n", my_uid);

    while(fgets(path, sizeof(path)-1, fp) != NULL){
        char line_copy[1035];
        strcpy(line_copy, path);
        
        char *user = strtok(line_copy, ":");
        strtok(NULL, ":"); 
        char *uid_str = strtok(NULL, ":");
        
        if(uid_str != NULL){
            int uid = atoi(uid_str);
            if(uid >= 1000 && uid != my_uid && strcmp(user, "nobody") != 0){
                printf("Користувач: %-15s | UID: %d\n", user, uid);
            }
        }
    }

    pclose(fp);
    return 0;
}