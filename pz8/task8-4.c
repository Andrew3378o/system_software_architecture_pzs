#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("Child process  (pid returned: %d)\n", pid);
    } else {
        printf("Parent process (pid returned: %d)\n", pid);
        wait(NULL);
    }

    return 0;
}