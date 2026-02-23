#include <stdio.h>
#include <time.h>

int main() {
    size_t time_size = sizeof(time_t);
    printf("Розмір time_t на цій архітектурі: %zu байт(ів)\n", time_size);

    time_t max_time = (time_t)(((unsigned long long)1 << (time_size * 8 - 1)) - 1);

    printf("Максимальне значення time_t: %lld секунд після Епохи Unix\n", (long long)max_time);

    struct tm *timeinfo = gmtime(&max_time);
    
    if (timeinfo != NULL) {
        printf("Цей час відповідає даті (UTC): %s", asctime(timeinfo));
    } else {
        printf("Значення занадто велике для стандартної функції gmtime()!\n");
    }

    return 0;
}