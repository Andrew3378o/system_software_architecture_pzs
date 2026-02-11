## Завдання 1.14
- Напишіть програму last, яка читає останні n рядків із файлу або стандартного вводу.
- Реалізуйте ефективне використання пам’яті (не завантажуйте весь файл у пам’ять, використовуйте кільцевий буфер).
- Додайте підтримку аргументів командного рядка (-n 10 для виведення останніх 10 рядків).
- Додайте опцію -r, яка виводить рядки у зворотному порядку.
- Оптимізуйте читання великих файлів (mmap, fgets).

## Технічна реалізація
В основі програми лежить алгоритм кільцевого буфера, що забезпечує виконання головної вимоги завдання — ефективне використання ресурсів. Програма не завантажує весь вміст файлу в оперативну пам'ять, що дозволяє обробляти файли будь-якого розміру. Замість цього динамічно виділяється масив вказівників фіксованого розміру $N$ (визначеного користувачем). Читання здійснюється потоково за допомогою функції ```fgets```. Кожен новий зчитаний рядок записується в буфер за поточним індексом. Якщо буфер вже заповнений, програма звільняє пам'ять найстарішого збереженого рядка в поточній позиції та перезаписує його новим, після чого індекс зміщується циклічно.

## Використання 
1. Відкрийте текстовий файл та вставте бажаний текст (в даному прикладі використовується файл ```expample.txt```);
2. Скомпілюйте програму у виконуваний файл командою ```gcc last.c -o last```;
3. Запустіть програму командою ```./last example.txt -n N -r```. Прапорець ```-n N``` вказує, що необхідно вивести останні ```N``` рядків тексту з текстового файлу (за замовчування програма виводить останні 10 рядків тексту). Прапорець ```-r``` вказує на виведення рядків у зворотньому порядку.

## Приклади використання з файлом
Дано початковий текстовий файл ```example.txt```:
```
To be, or not to be, that is the question:
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune,
Or to take arms against a sea of troubles
And by opposing end them. To die—to sleep,
No more; and by a sleep to say we end
The heart-ache and the thousand natural shocks
That flesh is heir to: 'tis a consummation
Devoutly to be wish'd. To die, to sleep;
To sleep, perchance to dream—ay, there's the rub:
For in that sleep of death what dreams may come,
When we have shuffled off this mortal coil,
Must give us pause—there's the respect
That makes calamity of so long life.
For who would bear the whips and scorns of time,
Th'oppressor's wrong, the proud man's contumely,
The pangs of dispriz'd love, the law's delay,
The insolence of office, and the spurns
That patient merit of th'unworthy takes,
When he himself might his quietus make
With a bare bodkin? Who would fardels bear,
To grunt and sweat under a weary life,
But that the dread of something after death,
The undiscovere'd country, from whose bourn
No traveller returns, puzzles the will,
And makes us rather bear those ills we have
Than fly to others that we know not of?
Thus conscience doth make cowards of us all,
And thus the native hue of resolution
Is sicklied o'er with the pale cast of thought,
And enterprises of great pith and moment
With this regard their currents turn awry
And lose the name of action.
```

При введення команди ```./last example.txt``` програма виведене останні 10 рядків тексту, тобто: 
```
The undiscovere'd country, from whose bourn
No traveller returns, puzzles the will,
And makes us rather bear those ills we have
Than fly to others that we know not of?
Thus conscience doth make cowards of us all,
And thus the native hue of resolution
Is sicklied o'er with the pale cast of thought,
And enterprises of great pith and moment
With this regard their currents turn awry
And lose the name of action. 
```

При введенні команди ```./last example.txt -n 2``` програма виведе останні 2 рядки тексту, тобто:
```
With this regard their currents turn awry
And lose the name of action.
```

При введенні команди ```./last example.txt -n 5 -r``` програма виведе останні 5 рядків тексту у зворотньому порядку, тобто:
```
And lose the name of action.
With this regard their currents turn awry
And enterprises of great pith and moment
Is sicklied o'er with the pale cast of thought,
And thus the native hue of resolution
```
## Приклад використання зі стандартним вводом

При введенні команди ```cat example.txt | ./last -n 3``` текст файлу ```example.txt``` передається у стандартний потік даних, де його використовує програма, виводячи останні три рядка, тобто:
```
And enterprises of great pith and moment
With this regard their currents turn awry
And lose the name of action.
```
При введенні команди ```ps aux | ./last -n 3``` в стандартний потік даних передається інформація про запущені процеси, де цю інформацію використовує програма, виводячи останні 3 рядків даних. Приклад виводу:
```
andrew3+     382  0.0  0.0   6072  4992 pts/1    S+   08:59   0:00 -bash
andrew3+   22566  0.0  0.0   8280  4096 pts/0    R+   10:03   0:00 ps aux
andrew3+   22567  0.0  0.0   2680  1152 pts/0    S+   10:03   0:00 ./last -n 3
```