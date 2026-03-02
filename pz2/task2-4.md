## Завдання 2.4
Ознайомтеся з виводом gstack і порівняйте його з GDB.
Приклад тестової програми:
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define MSG "\nIn function %20s; &localvar = %p\n"

static void bar_is_now_closed(void) {
    int localvar = 5;
    printf(MSG, __FUNCTION__, &localvar);
    printf("\n Now blocking on pause()...\n");
    pause();
}

static void bar(void) {
    int localvar = 5;
    printf(MSG, __FUNCTION__, &localvar);
    bar_is_now_closed();
}

static void foo(void) {
    int localvar = 5;
    printf(MSG, __FUNCTION__, &localvar);
    bar();
}

int main(int argc, char **argv) {
    int localvar = 5;
    printf(MSG, __FUNCTION__, &localvar);
    
    foo();
    
    return EXIT_SUCCESS;
}
```

## Технічна реалізація
GDB (GNU Debbuger) - переносимий відлагоджувач, який пропонує великий вибір засобів спостереження і контролю за виконанням комп'ютерних програм.
`gstack` - комадна, яка виводить стек виконуваної програми.
Тобто GDB є набагато більш загальним і універсальним інструментом для відлагодження програми, в той час як `gstack` фокусується лише на стекові.
Для практичного аналізу їх відмінностей пропонується дослідити виконання програми, код якої дано, з використанням цих двох різних інструментів.
Користувачі Ubuntu можуть зіткнутися з проблемою: на момент написання
(Ubuntu 18.04) gstack, схоже, не був доступний (альтернативою може бути
pstack). Тому тут дослідження було проведено за допомогою GDB.

## Використання
Для практичного застосування GDB необхідно ввести наступну послідовність команд:
1. ```gcc -g task2-4.c -o task2-4``` - компіляція програми у режимі відлагодження.
2. ```./task2-4 &``` - запуск програми у фоновому режимі, після чого виведеться його PID.
3. ```sudo gdb -q -p [PID]``` - підключення до активного процесу.
4. ```backtrace``` - вивід стекових кадрів програми.

Після закінчення дослідження і виходу з інтерфейсу відлагоджувальника необхідно виконати команду `kill [PID]`, щоб закінчити непотрібний фоновий процес.

## Приклад використання
Приклад виводу після завершення кроку 2:

```
andrew3378o@DESKTOP-N1R2BIO:/mnt/d/АСПЗ/pz2$ ./task2-4 &
[6] 37640
andrew3378o@DESKTOP-N1R2BIO:/mnt/d/АСПЗ/pz2$
In function                 main; &localvar = 0x7ffdabedc454

In function                  foo; &localvar = 0x7ffdabedc424

In function                  bar; &localvar = 0x7ffdabedc404

In function    bar_is_now_closed; &localvar = 0x7ffdabedc3e4

 Now blocking on pause()...
```
Приклад виводу після завершення кроку 3:
```
andrew3378o@DESKTOP-N1R2BIO:/mnt/d/АСПЗ/pz2$ sudo gdb -q -p 37640
Attaching to process 37640
Reading symbols from /mnt/d/АСПЗ/pz2/task2-4...
Reading symbols from /lib/x86_64-linux-gnu/libc.so.6...
Reading symbols from /usr/lib/debug/.build-id/8e/9fd827446c24067541ac5390e6f527fb5947bb.debug...
Reading symbols from /lib64/ld-linux-x86-64.so.2...
Reading symbols from /usr/lib/debug/.build-id/da/07864eb4c1b06504b8688d25d7e84759fe708d.debug...
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
0x00007e24f40fa3d4 in __libc_pause () at ../sysdeps/unix/sysv/linux/pause.c:29

warning: 29     ../sysdeps/unix/sysv/linux/pause.c: No such file or directory
(gdb) 
```
Прикрад виводу після завершення кроку 4:
```
(gdb) backtrace
#0  0x00007e24f40fa3d4 in __libc_pause () at ../sysdeps/unix/sysv/linux/pause.c:29
#1  0x000058b13a885209 in bar_is_now_closed () at task2-4.c:11
#2  0x000058b13a88526c in bar () at task2-4.c:17
#3  0x000058b13a8852cf in foo () at task2-4.c:23
#4  0x000058b13a885339 in main (argc=1, argv=0x7ffdabedc588) at task2-4.c:30
```
