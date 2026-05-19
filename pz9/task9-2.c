#include <stdio.h>
#include <stdlib.h>

int main() {
    int status = system("head -n 2 /etc/shadow 2>/dev/null");
    if(status != 0){
        printf("permission denied\n\n");
    }
    system("sudo head -n 2 /etc/shadow");
    
    return 0;
}