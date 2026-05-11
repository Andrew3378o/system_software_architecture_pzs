#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char payload[1024]; 
} HeavyStruct;

int cmp_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int cmp_double(const void *a, const void *b) {
    double arg1 = *(double*)a;
    double arg2 = *(double*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int cmp_struct(const void *a, const void *b) {
    return (((HeavyStruct*)a)->id - ((HeavyStruct*)b)->id);
}

void benchmark() {
    const int N = 5000;
    clock_t start, end;

    int *arr_int = malloc(N * sizeof(int));
    for(int i=0; i<N; i++) arr_int[i] = rand();
    
    start = clock();
    qsort(arr_int, N, sizeof(int), cmp_int);
    end = clock();
    printf("Сортування int (4 байта):       %f сек.\n", (double)(end - start) / CLOCKS_PER_SEC);

    double *arr_dbl = malloc(N * sizeof(double));
    for(int i=0; i<N; i++) arr_dbl[i] = (double)rand()/RAND_MAX;

    start = clock();
    qsort(arr_dbl, N, sizeof(double), cmp_double);
    end = clock();
    printf("Сортування double (8 байтів):   %f сек.\n", (double)(end - start) / CLOCKS_PER_SEC);

    HeavyStruct *arr_struct = malloc(N * sizeof(HeavyStruct));
    for(int i=0; i<N; i++) arr_struct[i].id = rand();

    start = clock();
    qsort(arr_struct, N, sizeof(HeavyStruct), cmp_struct);
    end = clock();
    printf("Сортування HeavyStruct (1 КБ):  %f сек.\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(arr_int); free(arr_dbl); free(arr_struct);
}

int main() {
    srand(time(NULL));
    printf("Аналіз продуктивності qsort для різних типів\n");
    benchmark();
    return 0;
}