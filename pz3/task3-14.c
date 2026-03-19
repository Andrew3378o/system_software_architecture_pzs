#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

void test_nofile() {
    int count = 0;
    while (1) {
        int fd = open("/dev/null", O_RDONLY);
        if (fd < 0) {
            perror("Помилка відкриття файлу");
            printf("Відкрито файлових дескрипторів: %d\n", count);
            break;
        }
        count++;
    }
}

void test_fsize() {
    signal(SIGXFSZ, SIG_IGN); 
    FILE *fp = fopen("test_large_file.tmp", "w");
    if (!fp) {
        perror("Помилка створення файлу");
        exit(1);
    }

    long long written = 0;
    const int chunk_size = 1024 * 1024; 
    char *buffer = malloc(chunk_size);
    if (!buffer) exit(1);

    while (written < 50 * 1024 * 1024) { 
        size_t res = fwrite(buffer, 1, chunk_size, fp);
        if (res < chunk_size) {
            perror("Помилка запису (перевищено fsize)");
            printf("Записано даних: %lld МБ\n", written / (1024 * 1024));
            break;
        }
        written += chunk_size;
    }
    
    if (written >= 50 * 1024 * 1024) {
        printf("Записано 50 МБ (ліміт не досягнуто).\n");
    }
    
    free(buffer);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Використання: %s [nofile|fsize]\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "nofile") == 0) {
        test_nofile();
    } else if (strcmp(argv[1], "fsize") == 0) {
        test_fsize();
    } else {
        fprintf(stderr, "Невідомий параметр: %s\n", argv[1]);
        return 1;
    }
    
    return 0;
}