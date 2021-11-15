#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct message
{
    int dest;
    char str[1024];
};

volatile sig_atomic_t sig1_flag = 0, sig2_flag = 0; 

void sig_handler (int sig);
int read_config (int child_index, struct message * msg);
int run_host (pid_t * children, int child_count, int gates[2]);
int run_client (int child_index, pid_t father_pid, int gates[2]);

int main (int argv, char ** argc)
{
    int child_count = 0;
    int i = 0;
    int father_pid = getpid(); 
    pid_t * children = NULL;
    int gates[2] = {0, 0};

    struct sigaction act;
    sigset_t set;
 
    if (argv < 2)
    {
        printf ("Process count required.\n");
        return 1;
    }

    errno = 0;
    child_count = (int) strtol (argc[1], NULL, 10) - 1;
    if (errno)
    {
        printf ("Error while reading process count: %s\n", strerror (errno));
        return 1;
    }
    else if (child_count < 0) 
    {
        printf ("Process count is less than 1\n");
        return 1;
    }
    else if (child_count == 0)
        return 0;

    children = (pid_t *) malloc (sizeof (pid_t) * child_count);

    assert (children);

    if (pipe (gates) == -1)
    {
        printf ("Error while opening pipe: %s", strerror (errno));
    }

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
            run_client (i, father_pid, gates);
            free (children);

            exit (0);
        }
        else
        {
            children[i] = child_pid;
        }
    }

    run_host (children, child_count, gates);

    free (children);
    return 0;
}

void sig_handler (int sig)
{
    if (sig == SIGUSR1)
        sig1_flag = 1;

    else if (sig == SIGUSR2)
        sig2_flag = 1;
}

int read_config (int child_index, struct message * msg)
{
    FILE * config = NULL;
    char filename[128] = "";
    char input = 0;
    int i = 0;

    sprintf (filename, "config_%d", child_index);

    config = fopen (filename, "r");
    if (!config)
    {
        printf ("[%d]: File open failed.\n", getpid());
        return 1;
    }
    
    if (fscanf (config, "%d", &(msg->dest)) != 1 || 
            ((input = fgetc (config)) != '\n'))
    {
        printf ("[%d]: Failed to read dest index\n", getpid());
        fclose (config);
        return 1;
    }

    for (i = 0; (input = fgetc (config)) != EOF; i++)
    {
        if (i >= 1024)
        {
            printf ("[%d]: Warning: Character limit exceeded.\n", getpid());
            break;
        }
        
        msg->str[i] = input;
    }

    fclose (config);

    return 0; 
}

int run_host (pid_t * children, int child_count, int gates[2])
{
    int i = 0;
    struct message msg;
    int dest_index = 0;

    for (i = 0; i < child_count; i++)
    {
        /** Sending request to the child **/
        kill (children[i], SIGUSR1);
        
        /** Waiting for an answer **/
        if (!sig1_flag)
            pause();
        sig1_flag = 0;

        /** Reading message **/
        read (gates[0], &msg, sizeof (struct message));

        /** If message is empty, continue cycle **/
        if (msg.dest == 0)
            continue;

        /** Sending transaction prepare request to the dest **/
        kill (msg.dest, SIGUSR2);

        /** Sending message **/
        write (gates[1], &msg, sizeof (struct message));

        /** Waiting transaction to complete **/
        if (!sig1_flag)
            pause();
        sig1_flag = 0;
    }  

    return 0;
}

int run_client (int child_index, pid_t father_pid, int gates[2])
{
    int message_delivered = 0;
    struct message msg;

    /** Reading process configuration file **/
    if (read_config (child_index, &msg))
        return 1;

    for 

    return 0;
} 
