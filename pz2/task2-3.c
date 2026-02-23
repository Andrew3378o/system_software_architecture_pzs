#include <stdio.h>
#include <stdlib.h>

int data_var = 10; 
int bss_var;       

void grow_stack() {
    int large_array[10000]; 
    printf("Нова вершина стека (в grow_stack): %p\n", (void*)&large_array);
}

int main() {
    int stack_var; 
    int *heap_var = malloc(sizeof(int)); 

    printf("Сегмент Text (код функції main):   %p\n", (void*)main);
    printf("Сегмент Data (data_var):           %p\n", (void*)&data_var);
    printf("Сегмент BSS  (bss_var):            %p\n", (void*)&bss_var);
    printf("Купа / Heap  (heap_var):           %p\n", (void*)heap_var);
    printf("Початкова вершина стека:           %p\n", (void*)&stack_var);
    
    grow_stack(); 

    free(heap_var);
    return 0;
}