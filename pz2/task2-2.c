#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *files[] = {"task2-2a", "task2-2b", "task2-2c", "task2-2d"};
    const char *descriptions[] = {
        "глобально неініціалізованими змінними",
        "глобально ініціалізованими змінними",
        "локально неініціалізованими змінними",
        "локально ініціалізованими змінними"
    };

    const char *flags[] = {"", "-O2", "-O3", "-g"};
    const char *flag_desc[] = {
        "Стандартна компіляція",
        "Оптимізація рівня -O2",
        "Оптимізація рівня -O3",
        "Компіляція для відлагодження"
    };

    char command[256];

    for (int i = 0; i < 4; i++) {
        printf("%s\n", flag_desc[i]);

        for (int j = 0; j < 4; j++) {
            printf("Розмір з %s (%s.c):\n", descriptions[j], files[j]);
            snprintf(command, sizeof(command), "gcc %s %s.c -o %s", flags[i], files[j], files[j]);
            int res = system(command);

            if (res == -1) {
                printf("Помилка під час компіляції %s\n\n", files[j]);
            } else {
                snprintf(command, sizeof(command), "size %s", files[j]);
                system(command);
                fflush(stdout);
                snprintf(command, sizeof(command), "ls -l %s", files[j]);
                system(command);
            }
            printf("\n");
        }
    }
    return 0;
}