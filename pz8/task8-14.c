#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void print_current_offset(int fd, const char *msg) {
    off_t current = lseek(fd, 0, SEEK_CUR);
    printf("[Offset Check] %s: поточна позиція у файлі = %ld\n", msg, (long)current);
}

int main() {
    const char *filename = "pread_demo.bin";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    
    unsigned char data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    write(fd, data, 10);
    
    lseek(fd, 0, SEEK_SET);
    printf("Початок експерименту з pread()\n");
    print_current_offset(fd, "Після відкриття та lseek(0)");

    unsigned char buf;

    pread(fd, &buf, 1, 5);
    printf("pread(offset 5): отримано значення %d\n", buf);
    print_current_offset(fd, "Після pread(5)");

    pread(fd, &buf, 1, 9);
    printf("pread(offset 9): отримано значення %d\n", buf);
    print_current_offset(fd, "Після pread(9)");

    pread(fd, &buf, 1, 0);
    printf("pread(offset 0): отримано значення %d\n", buf);
    print_current_offset(fd, "Після pread(0)");

    read(fd, &buf, 1);
    printf("Звичайний read(): отримано значення %d\n", buf);
    print_current_offset(fd, "Після звичайного read()");

    close(fd);
    unlink(filename);
    return 0;
}