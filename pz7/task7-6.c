#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_DIRS 4096

static int cmp_strings(const void *a, const void *b){
    return strcmp(*(const char *const *)a, *(const char *const *)b);
}

int main(){
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    char *dirs[MAX_DIRS];
    int count = 0;
    int i;

    dir = opendir(".");
    if(dir == NULL){
        perror("opendir");
        return 1;
    }

    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(lstat(entry->d_name, &st) == -1){
            fprintf(stderr, "lstat(%s): %s\n", entry->d_name, strerror(errno));
            continue;
        }

        if(S_ISDIR(st.st_mode)){
            if(count >= MAX_DIRS){
                fprintf(stderr, "Перевищено ліміт каталогів (%d)\n", MAX_DIRS);
                break;
            }
            dirs[count] = strdup(entry->d_name);
            if(dirs[count] == NULL){
                perror("strdup");
                closedir(dir);
                return 1;
            }
            count++;
        }
    }

    closedir(dir);

    qsort(dirs, (size_t)count, sizeof(char *), cmp_strings);

    printf("Підкаталоги у поточному каталозі (алфавітний порядок):\n");
    for(i = 0; i < count; i++){
        printf("  %s/\n", dirs[i]);
        free(dirs[i]);
    }

    if(count == 0) printf("  (підкаталоги відсутні)\n");

    return 0;
}
