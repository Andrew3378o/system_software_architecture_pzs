#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    FILE *src, *dst;
    char buffer[BUFFER_SIZE];
    size_t bytes;

    if (argc != 3) {
        printf("Program need two arguments\n");
        return 1;
    }

    src = fopen(argv[1], "rb");
    if (src == NULL) {
        printf("Cannot open file %s for reading\n", argv[1]);
        return 1;
    }

    dst = fopen(argv[2], "wb");
    if (dst == NULL) {
        printf("Cannot open file %s for writing\n", argv[2]);
        fclose(src);
        return 1;
    }

    while ((bytes = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
        if (fwrite(buffer, 1, bytes, dst) != bytes) {
            printf("File size limit exceeded or write error\n");
            fclose(src);
            fclose(dst);
            return 1;
        }
    }

    if (ferror(src)) {
        printf("Error while reading file\n");
    }

    fclose(src);
    fclose(dst);

    return 0;
}