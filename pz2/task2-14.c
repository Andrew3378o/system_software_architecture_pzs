#include <stdio.h>
#include <stdlib.h>

int global_var = 10; 

int main() {
    int local_var = 5;               
    void *heap_var = malloc(100);    

    printf("Адреса коду (main):     %p\n", (void*)main);
    printf("Адреса глобальної:      %p\n", (void*)&global_var);
    printf("Адреса купи (heap):     %p\n", heap_var);
    printf("Адреса стека (local):   %p\n", (void*)&local_var);

    char cmd[64];
    sprintf(cmd, "cat /proc/%d/maps", getpid());
    printf("address range             perms offset  dev   inode                      pathname\n");
    system(cmd);

    free(heap_var);
    return 0;
}