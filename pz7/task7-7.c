#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

static int is_c_source(const char *name){
    size_t len = strlen(name);
    return len > 2 && strcmp(name + len - 2, ".c") == 0;
}

static int read_answer(){
    int first = fgetc(stdin);
    int ch = first;
    while (ch != '\n' && ch != EOF) ch = fgetc(stdin);
    return first;
}

int main(){
    DIR *dir;
    struct dirent *entry;
    struct stat st;
    uid_t my_uid = getuid();

    dir = opendir(".");
    if(dir == NULL){
        perror("opendir");
        return 1;
    }

    printf("Ваші вихідні файли на C у поточному каталозі:\n\n");

    while((entry = readdir(dir)) != NULL){
        if(!is_c_source(entry->d_name)) continue;

        if(lstat(entry->d_name, &st) == -1){
            fprintf(stderr, "lstat(%s): %s\n", entry->d_name, strerror(errno));
            continue;
        }

        if(st.st_uid != my_uid) continue;

        printf("Файл: %-30s  (права: %04o)\n", entry->d_name, (unsigned)(st.st_mode & 0777));

        if(st.st_mode & S_IROTH){
            printf("  -> Інші вже мають право на читання.\n\n");
            continue;
        }

        printf("  Надати іншим право на читання? [y/N]: ");
        fflush(stdout);

        int ans = read_answer();
        if(ans == 'y' || ans == 'Y'){
            mode_t new_mode = st.st_mode | S_IROTH;
            if(chmod(entry->d_name, new_mode) == -1) fprintf(stderr, "  chmod: %s\n", strerror(errno));
            else printf("  -> Право на читання надано.\n");
        } 
        else{
            printf("  -> Без змін.\n");
        }
        putchar('\n');
    }

    closedir(dir);
    return 0;
}
