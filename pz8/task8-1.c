#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    fcntl(pipefd[1], F_SETFL, O_NONBLOCK);

    char buffer[100000];
    memset(buffer, 'A', sizeof(buffer));

    ssize_t count = write(pipefd[1], buffer, sizeof(buffer));

    if (count == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Помилка: Буфер переповнений, запис неможливий зараз.\n");
        } else {
            perror("write");
        }
    } else {
        printf("Запитувано: %zu байтів\n", sizeof(buffer));
        printf("Фактично записано: %zd байтів\n", count);
        if (count < sizeof(buffer)) {
            printf("Причина: Обмеження системного буфера конвеєра.\n");
        }
    }
    return 0;
}