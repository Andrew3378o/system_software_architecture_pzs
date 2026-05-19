#include <stdio.h>
#include <stdlib.h>

int main() {
    
    printf("Виконання команди 'whoami':\n");
    system("whoami");
    
    printf("\nВиконання команди 'id':\n");
    system("id");
    
    printf("\nСписок груп, до яких належить користувач:\n");
    system("id -Gn");
    
    return 0;
}