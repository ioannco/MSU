//
// Created by Ivan on 12/20/2021.
//

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <memory.h>
#include <stdlib.h>
#include <signal.h>

volatile sig_atomic_t flag = 0;

void handler (sig)
{
    flag = 1;
}

int main (int argc, char ** argv)
{
    int * array = NULL;     // указатель для выделения массива
    int step = 0;           // шаг
    int i = 0;              // итератор
    int N = 0;              // N
    pid_t son_pid = 0;      // pid сына
    int pipes[2];           // дескрипторы пайпа

    // проверяем аргументы
    if (argc < 2)
        return 1;

    // считываем числа из аргументов строки
    if (sscanf (argv[1], "%d", &N) != 1)
        return 1;
    if (sscanf (argv[2], "%d", &step) != 1)
        return 1;

    // проверям ввод
    if (N <= 0)
        return 1;
    if (step <= 0)
        return 1;

    // выделяем массив
    array = malloc (sizeof(int) * N);

    // считываем массив
    for (i = 0; i < N; i++)
        scanf ("%d", &array[i]);

    // ставим обработчик
    signal (SIGUSR1, handler);

    // создаем неименованный канал
    if (pipe (pipes) == -1)
        return 1;

    // форкаемся
    son_pid = fork ();
    if (son_pid == -1)
        return 1;

    // если отец
    if (son_pid)
    {
        // работаем в цикле
        while (1)
        {
            while (!flag)
            {/* ждем сигнала */}
            flag = 0;

            // читаем индекс из канала
            read (pipes[0], &i, sizeof (int));

            // если индекс вышел за границы
            if (i < 0)
            {
                // закрываем канал
                close (pipes[0]);
                close (pipes[1]);

                // говорим сыну чтобы он попробовал считать из канала и понял, что он закрыт, и сам завершился
                kill (son_pid, SIGUSR1);

                // ждем его завершения
                wait (NULL);

                break;
            }

            // печатаем индекс
            printf ("father %d\n", i);

            // сбрасываем буфер (хз зачем)
            fflush (stdout);

            // кладем элемент массива
            write (pipes[1], &array[i], sizeof(int));

            // посылаем сыну сигнал о том, что мы положили
            kill (son_pid, SIGUSR1);
        }
    }
    // если сын
    else
    {
        int arr_el = 0;
        i = N - 1; // инициализируем индекс

        while (1)
        {
            // пишем индекс
            write (pipes[1], &i, sizeof (int));

            // говорим отцу, что мы его записали
            kill (getppid(), SIGUSR1);

            while (!flag) {/* ждем отца */}
            flag = 0;

            // читаем элемент массива и проверяем, не закрыл ли отец канал
            if (read (pipes[0], &arr_el, sizeof (int)) == -1)
            {
                // если закрыл

                // закрываем нашу сторону
                close (pipes[0]);
                close (pipes[1]);

                // выходим из цикла
                break;
            }

            // печатаем его
            printf ("son %d\n", arr_el);

            // сбрасываем буфер (все еще хз зачем)
            fflush (stdout);

            // уменьшаем индекс
            i -= step;
        }
    }

    // обязательно освобождаем память
    free (array);

    return 0;
}