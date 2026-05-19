#include <stdio.h>
#include <stdlib.h>

int main(){
    const char *script_path = "/tmp/cron_simulate.sh";

    system("echo '#!/bin/bash' > /tmp/cron_simulate.sh");
    system("echo 'echo \"Поточний користувач: $(whoami)\"' >> /tmp/cron_simulate.sh");
    system("echo 'echo \"PATH середовища: $PATH\"' >> /tmp/cron_simulate.sh");
    system("echo 'ls /tmp > /dev/null && echo \"Команда ls виконана успішно\" || echo \"іПомилка ls\"' >> /tmp/cron_simulate.sh");
    
    system("echo 'custom_tool 2>/dev/null || echo \"Помилка: custom_tool не знайдено\"' >> /tmp/cron_simulate.sh");
    
    system("chmod +x /tmp/cron_simulate.sh");
    
    printf("Запуск у звичайному терміналі:\n");
    system("bash /tmp/cron_simulate.sh");
    
    printf("\nЗапуск в ізольованому середовищі (імітація cron):\n");
    system("env -i PATH=\"/usr/bin:/bin\" USER=\"nobody\" bash /tmp/cron_simulate.sh");

    system("rm -f /tmp/cron_simulate.sh");
    return 0;
}