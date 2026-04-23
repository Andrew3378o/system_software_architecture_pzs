#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

void execute_memory_allocation_test(int xa, int xb) {
    int num = xa * xb;
    
    printf("a: %d\n", xa);
    printf("b: %d\n", xb);
    printf("num: %d\n", num);
    printf("size_t: %zu\n", (size_t)num);
    
    void *memory_region = malloc(num);
    
    if (memory_region == NULL) {
        perror("System resource allocation failure");
    } else {
        printf("address: %p\n", memory_region);
        free(memory_region);
    }
}

int main() {
    printf("Phase 1:\n");
    execute_memory_allocation_test(-1, 1);
    
    printf("\nPhase 2:\n");
    execute_memory_allocation_test(50000, 50000);

    printf("\nPhase 3:\n");
    execute_memory_allocation_test(65536, 65537);
    
    return 0;
}