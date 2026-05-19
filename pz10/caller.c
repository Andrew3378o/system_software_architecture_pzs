#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    printf("[caller.c] Батьківський процес (PID: %d) починає роботу.\n", getpid());
    
    pid_t pid = fork(); 
    
    if(pid < 0){
        perror("Помилка fork()");
        exit(1);
    } 
    else if(pid == 0){
        printf("[caller.c] Дочірній процес (PID: %d) готується виконати exec()...\n", getpid());
        execl("./message", "message", NULL);
        
        perror("Помилка execl"); 
        exit(1);
    } 
    else{
        printf("[caller.c] Батьківський процес чекає на завершення дочірнього...\n");
        wait(NULL);
        printf("[caller.c] Дочірній процес завершився. Головна програма закінчує роботу.\n");
    }

    return 0;
}