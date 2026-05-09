#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

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
    int deleted = 0;
    int skipped = 0;

    dir = opendir(".");
    if(dir == NULL){
        perror("opendir");
        return 1;
    }

    printf("Інтерактивне видалення файлів у поточному каталозі.\n");
    printf("Відповідайте 'y' для видалення, 'a' для видалення ВСІХ, 'q' для виходу, або будь-яку іншу клавішу для пропуску.\n\n");

    int delete_all = 0;

    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        if(lstat(entry->d_name, &st) == -1){
            fprintf(stderr, "lstat(%s): %s\n", entry->d_name, strerror(errno));
            continue;
        }

        if(!S_ISREG(st.st_mode)){
            printf("  [каталог/інше] %s — пропускається\n", entry->d_name);
            continue;
        }

        if(delete_all){
            if(unlink(entry->d_name) == -1) fprintf(stderr, "unlink(%s): %s\n", entry->d_name, strerror(errno));
            else{
                printf("  Видалено: %s\n", entry->d_name);
                deleted++;
            }
            continue;
        }

        printf("Видалити '%s'? [y/a/q/N]: ", entry->d_name);
        fflush(stdout);

        int ans = read_answer();

        switch(ans){
        case 'y': case 'Y':
            if(unlink(entry->d_name) == -1) fprintf(stderr, "  unlink: %s\n", strerror(errno));
            else{
                printf("  -> Видалено.\n");
                deleted++;
            }
            break;

        case 'a': case 'A':
            delete_all = 1;
            if(unlink(entry->d_name) == -1) fprintf(stderr, "  unlink: %s\n", strerror(errno));
            else{
                printf("  -> Видалено (режим «всі»).\n");
                deleted++;
            }
            break;

        case 'q': case 'Q':
            printf("  -> Вихід.\n");
            goto done;

        default:
            printf("  -> Пропущено.\n");
            skipped++;
            break;
        }
    }

done:
    closedir(dir);
    printf("\nПідсумок: видалено %d, пропущено %d.\n", deleted, skipped);
    return 0;
}
