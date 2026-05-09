#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COUNT 10  

static double rand_0_1(){
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

static double rand_0_n(double n){
    return rand_0_1() * n;
}

int main(void) {
    int i;
    double n;

    srand((unsigned int)time(NULL));

    printf("(a) %d випадкових чисел у діапазоні [0.0, 1.0]:\n", COUNT);
    for(i = 0; i < COUNT; i++) printf("  %2d: %.8f\n", i + 1, rand_0_1());

    printf("\nВведіть n (верхня межа діапазону): ");
    if(scanf("%lf", &n) != 1){
        fprintf(stderr, "Помилка зчитування числа.\n");
        return 1;
    }

    printf("\n(b) %d випадкових чисел у діапазоні [0.0, %.4f]:\n", COUNT, n);
    for(i = 0; i < COUNT; i++) printf("  %2d: %.8f\n", i + 1, rand_0_n(n));

    return 0;
}
