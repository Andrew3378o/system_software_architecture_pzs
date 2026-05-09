#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 20

static int wait_for_key(void){
    int ch;
    int flush; 
    
    fprintf(stderr, "--Більше-- (натисніть будь-яку клавішу; 'q' для виходу) ");
    fflush(stderr);

    ch = fgetc(stdin);     

    flush = ch;
    while(flush != '\n' && flush != EOF) flush = fgetc(stdin);

    return (ch == 'q' || ch == 'Q') ? 0 : 1; 
}

static int display_file(const char *filename){
    FILE *fp;
    char  line[4096];
    int   line_count = 0;

    fp = fopen(filename, "r");
    if(fp == NULL){
        fprintf(stderr, "Не вдалося відкрити файл '%s': ", filename);
        perror("");
        return 0;
    }

    printf("==> %s <==\n", filename);

    while(fgets(line, sizeof(line), fp) != NULL){
        fputs(line, stdout);
        if(strchr(line, '\n') != NULL){
            line_count++;
        }

        if(line_count >= PAGE_SIZE){
            line_count = 0;
            if(!wait_for_key()){
                fclose(fp);
                return 0;
            }
        }
    }

    if(ferror(fp)){
        perror("fgets");
    }

    fclose(fp);
    return 1;
}

int main(int argc, char *argv[]){
    int i;

    if(argc < 2){
        fprintf(stderr, "Використання: %s <файл1> [файл2 ...]\n", argv[0]);
        return 1;
    }

    for(i = 1; i < argc; i++){
        if(!display_file(argv[i])) break;
    }

    return 0;
}
