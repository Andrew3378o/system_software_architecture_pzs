#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    int arch_bits = sizeof(void*) * 8;
    
    size_t max_size = SIZE_MAX;
    size_t max_ptrdiff = PTRDIFF_MAX;

    printf("Архітектура: %d-bit\n", arch_bits);
    printf("SIZE_MAX: %zu байтів\n", max_size);
    printf("PTRDIFF_MAX: %zu байтів\n\n", max_ptrdiff);

    printf("malloc(SIZE_MAX)...\n");
    void *ptr1 = malloc(max_size);
    if (ptr1) {
        printf("Успіх! (Це не має статися на сучасних ОС)\n");
        free(ptr1);
    } else {
        printf("Помилка: не вдалося виділити SIZE_MAX.\n");
    }

    printf("\nmalloc(PTRDIFF_MAX)...\n");
    void *ptr2 = malloc(max_ptrdiff);
    if (ptr2) {
        printf("Успіх! (Можливо на 32-bit, але малоймовірно на 64-bit через ліміти заліза)\n");
        free(ptr2);
    } else {
        printf("Помилка: не вдалося виділити PTRDIFF_MAX.\n");
    }

    printf("\nПошук практичного ліміту віртуальної пам'яті...\n");
    
    size_t chunk_size = (size_t)1024 * 1024 * 1024; // 1 GB
    size_t total_allocated = 0;
    void *last_successful_ptr = NULL;

    for (int i = 0; i < 200; i++) {
        void *test_ptr = malloc(total_allocated + chunk_size);
        if (test_ptr) {
            total_allocated += chunk_size;
            free(last_successful_ptr);
            last_successful_ptr = test_ptr;
        } else {
            break;
        }
    }

    printf("Практично вдалося зарезервувати одним шматком: %zu GB\n", total_allocated / (1024 * 1024 * 1024));
    
    if (last_successful_ptr) {
        free(last_successful_ptr);
    }

    return 0;
}