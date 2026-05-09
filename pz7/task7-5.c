#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#define PATH_MAX_LEN 4096

static void list_recursive(const char *base_path){
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char full_path[PATH_MAX_LEN];

    dir = opendir(base_path);
    if(dir == NULL){
        fprintf(stderr, "opendir(%s): %s\n", base_path, strerror(errno));
        return;
    }

    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", base_path, entry->d_name);

        if(lstat(full_path, &st) == -1){
            fprintf(stderr, "lstat(%s): %s\n", full_path, strerror(errno));
            continue;
        }

        printf("%s\n", full_path);

        if(S_ISDIR(st.st_mode)){
            list_recursive(full_path);
        }
    }

    closedir(dir);
}

int main(){
    list_recursive(".");
    return 0;
}
