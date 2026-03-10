## Завдання 3.3
Напишіть програму, що імітує кидання шестигранного кубика. Імітуйте кидки, результати записуйте у файл, для якого попередньо встановлено обмеження на його максимальний розмір (max file size). Коректно обробіть ситуацію перевищення ліміту.

## Технічна реалізація 
Технічна реалізація програми базується на використанні стандарту POSIX для керування ресурсами процесу та обробки сигналів операційної системи, де за допомогою системного виклику `setrlimit` та структури `struct rlimit` встановлюється жорстке обмеження `RLIMIT_FSIZE` на максимальний розмір створюваного файлу у 64 байти. Програма ініціалізує генератор випадкових чисел `srand(time(NULL))` для імітації кидків кубика, відкриває файл `result.txt` через низькорівневий дескриптор `open` з прапорцями запису та очищення, після чого у циклі форматує результати за допомогою `sprintf` і записує їх через `write`. Ключовим аспектом архітектури є реєстрація обробника сигналу `SIGXFSZ` через функцію `signal`, що дозволяє ядру ОС миттєво перервати виконання циклу при досягненні ліміту байтів і передати керування функції `handle_signal`.

## Приклад використання програми
Для запуску програми потрібно спочатку скомпілювати програму командою `gcc task3-3.c -o task3-3` та запустити командою `./task3-3 [кількість кидків кубика]`. 

Наприклад після команди `./task 3-3 3` можна побачити наступний вивід:
```
andrew3378o@DESKTOP-N1R2BIO:/mnt/d/АСПЗ/pz3$ ./task3-3 3
writing to the file ./task3-3 roll #1 with a value of 1
writing to the file ./task3-3 roll #2 with a value of 3
writing to the file ./task3-3 roll #3 with a value of 4
Success! Exiting the programm!
```
Вивід після команди `./task3-3 15`:
```
andrew3378o@DESKTOP-N1R2BIO:/mnt/d/АСПЗ/pz3$ ./task3-3 15
writing to the file ./task3-3 roll #1 with a value of 1
writing to the file ./task3-3 roll #2 with a value of 3
writing to the file ./task3-3 roll #3 with a value of 3
writing to the file ./task3-3 roll #4 with a value of 3
writing to the file ./task3-3 roll #5 with a value of 2
writing to the file ./task3-3 roll #6 with a value of 4
writing to the file ./task3-3 roll #7 with a value of 2
writing to the file ./task3-3 roll #8 with a value of 2
writing to the file ./task3-3 roll #9 with a value of 6
writing to the file ./task3-3 roll #10 with a value of 5
writing to the file ./task3-3 roll #11 with a value of 3

File limit reached!
```

Для того, щоб переконатись в тому, що файл дійсно має обмежений розмір, скористаємось командою `ls -l result.txt`, після якої можна побачити наступний вивід:
```
andrew3378o@DESKTOP-N1R2BIO:/mnt/d/АСПЗ/pz3$ ls -l result.txt
-rwxrwxrwx 1 andrew3378o andrew3378o 64 Mar 10 21:44 result.txt
```