#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

int main(int argc, char *argv[]){
    int n = 10;
    int reverse_flag = 0;
    char *filename = NULL;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-n") == 0){
            if(i + 1 < argc){
                n = atoi(argv[i+1]);
                i++;
            }
        } else if(strcmp(argv[i], "-r") == 0){
            reverse_flag = 1;
        } else {
            filename = argv[i];
        }
    }

    if(n <= 0){
    	fprintf(stderr, "Не правильне число рядків!\n");
        return 1;
    }

    FILE *fp;
    if(filename){
        fp = fopen(filename, "r");
        if(!fp){
            fprintf(stderr, "Помилка відкриття файлу!\n");
            return 1;
        }
    } else {
        fp = stdin;
    }

    char **lines = malloc(n * sizeof(char*));
    if(!lines){
        fprintf(stderr, "Помилка виділення пам'яті!\n");
        if(filename) fclose(fp);
        return 1;
    }

    for(int i = 0; i < n; i++){
        lines[i] = NULL;
    }

    char buffer[MAX];
    int current = 0;
    int total_read = 0;

    while(fgets(buffer, MAX, fp)){
        if(lines[current]){
            free(lines[current]);
        }
        
        lines[current] = malloc(strlen(buffer) + 1);
        strcpy(lines[current], buffer);

        current++;
        if(current >= n) current = 0; 
        total_read++;
    }

    int count = (total_read < n) ? total_read : n;
    int start = (total_read < n) ? 0 : current;

    if(reverse_flag){
        for(int i = 0; i < count; i++){
            int idx = (start + count - 1 - i) % n;
            if(lines[idx]) printf("%s", lines[idx]);
        }
    } else {
        for(int i = 0; i < count; i++){
            int idx = (start + i) % n;
            if(lines[idx]) printf("%s", lines[idx]);
        }
    }

    for(int i = 0; i < n; i++){
        if(lines[i]) free(lines[i]); 
    }

    free(lines);
    if(filename) fclose(fp);
    
    return 0;
}