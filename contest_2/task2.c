//
// Created by Ivan on 12/13/2021.
//

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

int flg = 0;

void handler (int sig)
{
    flg = 1;
}


int main(int argc, char ** argv)
{
    pid_t pid = -1;
    int nums[20];
    int pipes[2];
    int fd1, fd2;

    pipe (pipes);
    pid = fork ();

    signal (SIGUSR1, handler);

    if (pid)
    {
        fd1 = open (argv[1], O_RDONLY);
        fd2 = open (argv[2], O_WRONLY | O_CREAT, 0777);

        while (read (fd1, nums, sizeof (int) * 20) > 0)
        {
            close (pipes[0]);
            close (pipes[1]);
            close (fd1);
            close (fd2);
            kill (pid, SIGINT);
            wait (NULL);
            return 0;

            write (pipes[1], nums, sizeof (int) * 20);

            while (!flg);

            read (pipes[0], nums, sizeof (int) * 20);
            write (fd2, nums, sizeof (int) * 20);

        }
    }
    else
    {
        while (1)
        {
            int i = 0;
            if (read (pipes[0], nums, sizeof (int) * 20) <= 0)
            {
                close (pipes[0]);
                close (pipes[1]);
                return 0;
            }

            for (i = 0; i < 20; i++)
                if (nums[i] % 2 == 1)
                    nums[i] = 0;

            write (pipes[1], nums, sizeof (int) * 20);
            kill (SIGUSR1, getppid());
        }
    }

    return 0;
}