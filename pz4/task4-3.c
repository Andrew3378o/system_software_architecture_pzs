#include <stdio.h>
#include <stdlib.h>

void* test_malloc(size_t size, int force_null_on_zero) {
    if (size == 0 && force_null_on_zero) {
        return NULL;
    }

    return malloc(size);
}

int main(void) {
    printf("native behavior:\n");
    void *ptr_valid = test_malloc(0, 0);
    
    if (ptr_valid != NULL) {
        printf("result: valid pointer %p\n", ptr_valid);
        free(ptr_valid);
        printf("free(%p) executed safely.\n", ptr_valid);
    }

    printf("\nalternative standard behavior:\n");
    void *ptr_null = test_malloc(0, 1);
    
    if (ptr_null == NULL) {
        printf("result: NULL\n");
        free(ptr_null);
        printf("free(NULL) executed safely.\n");
    }

    return 0;
}