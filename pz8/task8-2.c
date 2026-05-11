#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *filename = "data.bin";
    unsigned char initial_data[] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};
    
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd, initial_data, 10);

    unsigned char buffer[4];
    off_t offset = lseek(fd, 3, SEEK_SET);
    
    ssize_t bytes_read = read(fd, buffer, 4);

    printf("Поточна позиція після lseek: %ld\n", (long)offset);
    printf("Зчитано байтів: %zd\n", bytes_read);
    printf("Вміст буфера (HEX): %02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);

    close(fd);
    unlink(filename);
    return 0;
}