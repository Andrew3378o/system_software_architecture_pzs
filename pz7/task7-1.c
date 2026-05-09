#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *rwho_pipe;
    FILE *more_pipe;
    char buffer[256];

    rwho_pipe = popen("rwho 2>/dev/null || echo '(rwho недоступний — показуємо who)';" "who", "r");
    if(rwho_pipe == NULL){
        perror("popen (rwho)");
        return EXIT_FAILURE;
    }

    more_pipe = popen("more", "w");
    if(more_pipe == NULL){
        perror("popen (more)");
        pclose(rwho_pipe);
        return EXIT_FAILURE;
    }

    while(fgets(buffer, sizeof(buffer), rwho_pipe) != NULL){
        if(fputs(buffer, more_pipe) == EOF){
            perror("fputs");
            break;
        }
    }

    pclose(rwho_pipe);
    pclose(more_pipe);

    return 0;
}
