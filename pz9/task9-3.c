#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    const char *target_file = "./root.txt";
    
    system("echo 'Початковий текст користувача' > /tmp/tmp.txt");
    
    printf("Копіюємо файл від імені root та змінюємо власника\n");
    system("sudo cp /tmp/tmp.txt ./root.txt");
    system("sudo chown root:root ./root.txt");
    system("sudo chmod 644 ./root.txt");
    
    printf("Поточний стан файлу:\n");
    system("ls -l ./root.txt");

    printf("\nСпроба дозапису у файл від звичайного користувача:\n");
    int write_status = system("echo 'Новий текст' >> ./root.txt 2>/dev/null");
    if(write_status != 0){
        printf("Permission denied\n");
    }
    
    printf("\nСпроба видалення файлу:\n");
    if(unlink(target_file) == 0){
        printf("Файл видалено.\n");
    }
    else{
        printf("Помилка: не вдалося видалити файл\n");
    }

    system("rm -f /tmp/tmp.txt");
    return 0;
}