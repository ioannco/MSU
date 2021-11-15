#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

volatile sig_atomic_t sig1_flag = 0, sig2_flag = 0; 

void sig_handler (int sig);

int main ()
{
    const int child_count = 2;
    int i = 0;
    int father_pid = getpid(); 
    pid_t * children = (pid_t *) malloc (sizeof (pid_t) * child_count);
    int gates[2] = {0, 0};

    const char * msg[2] = {"Hello!", "Hey!"};

    struct sigaction act;
    sigset_t set;

    pipe (gates);

    memset (&act, 0, sizeof (act));
    act.sa_handler = sig_handler;

    sigemptyset (&set);
    sigaddset (&set, SIGUSR1);
    sigaddset (&set, SIGUSR2);
    
    act.sa_mask = set;

    sigaction (SIGUSR1, &act, 0);
    sigaction (SIGUSR2, &act, 0);

    for (i = child_count; i > 0; i--)
    {
        pid_t child_pid = fork();

        if (!child_pid)
        {
            free (children);
            children = NULL;
            break;
        }
        else
        {
            printf ("[%d]: Adding child PID %d\n", getpid(), child_pid);
            children[i - 1] = child_pid;
        }
    }

    if (i)
    {
        printf ("[%d]: Waiting request from father.\n", getpid());

        if (!sig1_flag) 
            pause();
        printf ("[%d]: Request recieved\n", getpid());

        write (gates[1], msg[i - 1], 16);

        printf ("[%d]: Sending answer to father\n", getpid());
        kill (father_pid, SIGUSR1);
    }
    else
    {
        for (int i = 0; i < child_count; i++)
        {
            char buffer[16];

            printf ("[%d]: Sending request to the child with PID %d\n", getpid(), children[i]);
            kill (children[i], SIGUSR1);
            printf ("[%d]: Waiting for an answer..\n", getpid());

            if (!sig1_flag)
                pause();

            read (gates[0], buffer, 16);

            printf ("[%d]: Answer recieved: %s\n", getpid(), buffer);
        }

        for (int i = 0; i < child_count; i++)
        {
            printf ("[%d]: Waiting for child to die\n", getpid());
            printf ("[%d]: Wait returned %d\n", getpid(), wait(NULL));
        }

        free (children); 
    }

    return 0;
}

void sig_handler (int sig)
{
    if (sig == SIGUSR1)
        sig1_flag = 1;
    else if (sig == SIGUSR2)
        sig2_flag = 1;
}
