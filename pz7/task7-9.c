#define _POSIX_C_SOURCE 199309L 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

static double now_ms(){
    struct timespec ts;
    if(clock_gettime(CLOCK_MONOTONIC, &ts) == -1){
        perror("clock_gettime");
        exit(1);
    }
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1.0e6;
}

static volatile double result = 0.0;

static void code_under_test(){
    long i;
    double sum = 0.0;
    for(i = 0; i < 10000000L; i++) sum += sqrt((double)i);
    result = sum;
}

int main(){
    double t_start, t_end, elapsed;

    printf("Вимірювання часу виконання фрагмента коду...\n");

    t_start = now_ms();

    code_under_test();        

    t_end  = now_ms();
    elapsed = t_end - t_start;

    printf("Результат обчислення: %.2f  (щоб уникнути оптимізації)\n", result);
    printf("Час виконання: %.3f мс\n", elapsed);

    return 0;
}
