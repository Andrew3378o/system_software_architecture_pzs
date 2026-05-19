#include <stdio.h>
#include <stdlib.h>

int main() {
    
    printf("Домашній каталог\n");
    system("ls -ld ~");
    printf("Системні бінарні файли (/usr/bin)\n");
    system("ls -ld /usr/bin");
    printf("Конфігурації (/etc)\n");
    system("ls -ld /etc");
    
    printf("Спроба створити файл у /usr/bin:\n");
    system("touch /usr/bin/test_c_file 2>&1");
    
    printf("\nСпроба прочитати /etc/shadow:\n");
    system("head -n 1 /etc/shadow 2>&1");
    
    printf("\nСтворюємо скрипт без прав на виконання (-x):\n");
    system("echo 'echo \"Привіт з нездійсненного скрипта!\"' > /tmp/no_x_script.sh");
    system("chmod -x /tmp/no_x_script.sh");
    
    printf("Спроба прямого запуску (./script.sh):\n");
    system("/tmp/no_x_script.sh 2>&1");
    
    printf("Спроба запуску через інтерпретатор (bash script.sh):\n");
    system("bash /tmp/no_x_script.sh");
    
    system("rm -f /tmp/no_x_script.sh");
    return 0;
}