#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_access(const char* filepath){
    int r = access(filepath, R_OK);
    int w = access(filepath, W_OK);
    
    printf("Читання: %s | Запис: %s\n", (r == 0) ? "+" : "-", (w == 0) ? "+" : "-");
}

int main(){
    char tmpl[] = "/tmp/testmatrix_XXXXXX";
    int fd = mkstemp(tmpl);
    if(fd == -1){
        perror("Не вдалося створити тимчасовий файл");
        return 1;
    }
    close(fd);
    
    printf("Створено тимчасовий файл: %s\n\n", tmpl);
    
    char cmd[256];
    
    snprintf(cmd, sizeof(cmd), "sudo chown root:root %s", tmpl);
    system(cmd);
    
    printf("Маска 600 (Тільки власник root має rw):\n");
    snprintf(cmd, sizeof(cmd), "sudo chmod 600 %s", tmpl);
    system(cmd);
    check_access(tmpl);
    
    printf("\nМаска 644 (root - rw, інші - r):\n");
    snprintf(cmd, sizeof(cmd), "sudo chmod 644 %s", tmpl);
    system(cmd);
    check_access(tmpl);
    
    printf("\nМаска 666 (Всі користувачі мають rw):\n");
    snprintf(cmd, sizeof(cmd), "sudo chmod 666 %s", tmpl);
    system(cmd);
    check_access(tmpl);

    snprintf(cmd, sizeof(cmd), "sudo rm -f %s", tmpl);
    system(cmd);
    
    return 0;
}