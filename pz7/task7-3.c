#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    FILE *fp;
    char line[4096];
    int line_num   = 0;
    int found_any  = 0;

    if(argc != 3){
        fprintf(stderr, "Використання: %s <слово> <файл>\n", argv[0]);
        return 1;
    }

    const char *word = argv[1];
    const char *filename = argv[2];

    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr, "Не вдалося відкрити файл '%s': ", filename);
        perror("");
        return 1;
    }

    while(fgets(line, sizeof(line), fp) != NULL){
        line_num++;
        if(strstr(line, word) != NULL){
            printf("%d: %s", line_num, line);
            if(line[strlen(line) - 1] != '\n') putchar('\n');
            found_any = 1;
        }
    }

    if(ferror(fp)){
        perror("fgets");
        fclose(fp);
        return 1;
    }

    fclose(fp);

    if(!found_any){
        fprintf(stderr, "Слово '%s' не знайдено у файлі '%s'.\n", word, filename);
        return 1;
    }

    return 0;
}
