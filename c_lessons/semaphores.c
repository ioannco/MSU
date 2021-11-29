/*
 * Семафоры 29/11/2021
 *
 * Семафор - неотрицательная целочисленная переменная, над которой определены две атомарные операции:
 *
 * 1) down (P) - если семафор > 0, то уменьшает значение на 1.
 * Если = 0, то блокирует процесс до тех пор, пока он не увеличится.
 *
 * 2) up (V) - увеличивает семафор на 1
 */

/* библиотека IPC - <sys/ipc.h> */
#include <sys/ipc.h>

#ifndef _SYS_IPC_H

typedef int key_t;

/**
 * @brief получить IPC ключ к файлу
 **/
__attribute__((unused))
key_t ftok (char * filename, int num); /* -> ключ файла */

/**
 * @brief получить семафор (semid)
 *
 * IPC_PRIVATE - другие процессы не смогут подключиться через semget
 * IPC_CREAT | 0666 - создать массив семафоров
 */
__attribute__((unused))
int semget (key_t key, int nsem, int flags); /* -> id семафора */

struct semid_ds
{
    /* ... */
};

union semun
{
    int val;
    unsigned short * array;
    struct semid_ds buf;
};

/**
 * @brief управление семафорами
 *
 * cmd IPC_STAT - скопировать параметры семафора по адресу arg.buf
 *     IPC_SET  - установить параметры семафора по arg.buf
 *     IPC_RMID - удалить массив семафоров
 *     SETVAL, GETVAL - установить или получить значение переменной семафора
 *     SETALL, GETALL - (SET)GETVAL для всех элементов массива
 **/
__attribute__((unused))
int semctl (int semid, int num, int cmd, union semun arg);

struct sembuf
{
    short sem_num; /* номер семафора */
    short sem_op;  /* операция       */
    short sem_flg;
    /*
     * IPC_NOWAIT - запретить блокировку
     * SEM_UNDO   - вернуть исходное значение если процесс умер
     */
};

/**
 * @brief операция над семафором
 *
 * если sem_val + sem_op >= 0 то sem_val = sem_val + sem_op
 * иначе блокировка
 */
__attribute__((unused))
int semop (int semd, struct sembuf * so, int sems);

#endif /* _SYS_IPC_H */

#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>

int example1 ();

int main ()
{

}

int example1 ()
{
    int fd[2];
    struct sembuf P = {0, -1, 0}, V = {0, 1, 0};
    key_t key = ftok ("a.txt", 'a');
    int semid = semget (key, 1, IPC_CREAT | 0666 | IPC_PRIVATE);

    semctl (semid, 0, SETVAL, 0);

    pipe (fd);

    if (!fork ())
    {
        int snd = 7, rec;

        write (fd[1], &snd, sizeof (int));
        semop (semid, &P, 1);
        read (fd[0], &rec, sizeof (int));

        close (fd[0]);
        close (fd[1]);
    }
    else
    {
        int rec, snd = 5;

        read (fd[0], &rec, sizeof (int));
        write (fd[1], &snd, sizeof (int));
        semop (semid, &V, 1);

        close (fd[0]);
        close (fd[1]);

        wait (NULL);
    }

    return 0;
}


