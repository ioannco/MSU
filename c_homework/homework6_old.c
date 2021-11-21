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

#define DUMP_PID if (logs) printf ("[%d]: ", getpid());

struct message
{
    int dest, src;
    char str[1024];
};

volatile sig_atomic_t sig1_flag = 0, sig2_flag = 0, term_flag = 0; 

int logs = 0;

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
    
    /** Checking for the arguments **/ 
    if (argv < 2)
    {
        printf ("Process count required.\n");
        return 1;
    }

    /** Reading process count from args and handling errors **/
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

    if (argv > 2)
        logs = 1;

    DUMP_PID; 
    if (logs) printf ("Father started. Process count = %d\n", child_count + 1);

    /** Creating a children PID array **/
    children = (pid_t *) malloc (sizeof (pid_t) * child_count);

    assert (children);

    if (pipe (gates) == -1)
    {
        printf ("Error while opening pipe: %s", strerror (errno));
    }
    
    /*
    memset (&act, 0, sizeof (act));
    act.sa_handler = sig_handler;

    sigemptyset (&set);
    sigaddset (&set, SIGUSR1);
    sigaddset (&set, SIGUSR2);
    sigaddset (&set, SIGCHLD);
    sigaddset (&set, SIGTERM);
    
    act.sa_mask = set;

    sigaction (SIGUSR1, &act, 0);
    sigaction (SIGUSR2, &act, 0);
    sigaction (SIGCHLD, &act, 0);
    sigaction (SIGTERM, &act, 0);
    */

    signal (SIGUSR1, sig_handler);
    signal (SIGUSR2, sig_handler);
    signal (SIGCHLD, sig_handler);
    signal (SIGTERM, sig_handler);
   
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

    close (gates[0]);
    close (gates[1]);

    return 0;
}

void sig_handler (int sig)
{
    if (logs) printf ("[%d]: %s recieved\n", getpid(), strsignal (sig));

    if (sig == SIGCHLD)
    {
        wait (NULL);
        sig1_flag = 1;
    }
    else if (sig == SIGUSR1)
        sig1_flag = 1;
    else if (sig == SIGUSR2)
        sig2_flag = 1;
    else if (sig == SIGTERM)
        term_flag = 1;

}

int read_config (int child_index, struct message * msg)
{
    FILE * config = NULL;
    char filename[128] = "";
    char input = 0;
    int i = 0;

    /** Creating filename string **/
    sprintf (filename, "config_%d", child_index);

    /** Trying to open config file. On error return 1 **/
    config = fopen (filename, "r");
    if (!config)
    {
        printf ("[%d]: File open failed.\n", getpid());
        return 1;
    }

    /** Setting source child index **/
    msg->src = child_index - 1;
    
    /** Reading destination child index. On error return 1 **/
    if (fscanf (config, "%d", &(msg->dest)) != 1 || 
            ((input = fgetc (config)) != '\n'))
    {
        printf ("[%d]: Failed to read dest index\n", getpid());
        fclose (config);
        return 1;
    }

    msg->dest--;

    /** Writing message in the package **/
    for (i = 0; (input = fgetc (config)) != EOF; i++)
    {
        if (i >= 1023)
        {
            printf ("[%d]: Warning: Character limit exceeded.\n", getpid());
            break;
        }
        
        msg->str[i] = input;
    }

    /** Setting end of message symbol **/
    msg->str[i] = '\0';

    /** Closing file **/
    fclose (config);

    return 0; 
}

int run_host (pid_t * children, int child_count, int gates[2])
{
    int i = 0;
    struct message msg = {0, 0, ""};
    int dest_index = 0;

    DUMP_PID;
    if (logs) printf ("Host started.\n");

    for (i = 0; i < child_count; i++)
    {
        DUMP_PID;
        if (logs) printf ("Requesting child for a message from child %d\n", i);

        /** Sending request to the child. If process does not exist, continue requesting others **/
        if (kill (children[i], SIGUSR1) == -1) 
            continue;
        
        /** Waiting for an answer **/
        if (!sig1_flag)
            pause();
        sig1_flag = 0;

        /** Reading message **/
        assert (read (gates[0], &msg, sizeof (struct message)) != -1);

        DUMP_PID;
        if (logs) printf ("Answer from child recieved. src = %d, dest = %d\n", msg.src, msg.dest);

        /** If message is empty, continue cycle **/
        if (msg.dest == -1)
            continue;

        DUMP_PID;
        if (logs) printf ("Sending prepare request to child %d\n", i);

        /** Sending transaction prepare request to the dest **/
        if (kill (msg.dest, SIGUSR2) == -1)
            continue;

        /** Sending message **/
        assert (write (gates[1], &msg, sizeof (struct message)) != -1);

        /** Waiting transaction to complete **/
        if (!sig1_flag)
            pause();
        sig1_flag = 0;

        DUMP_PID;
        if (logs) printf ("Transaction to the child %d completed\n", i);
    }  

    for (int i = 0; i < child_count; i++)
    {
        DUMP_PID;
        if (logs) printf ("Terminating child %d\n", i);

        /** Sending exit signal for the child **/
        if (kill (children[i], SIGTERM) == -1)
            continue;
        /** P.S. really sorry about murdering children **/ 
   
        wait (NULL);
    }

    return 0;
}

int run_client (int child_index, pid_t father_pid, int gates[2])
{
    struct message msg;

    DUMP_PID;
    if (logs) printf ("Reading config.\n");

    /** Reading process configuration file **/
    if (read_config (child_index, &msg))
        return 1;

    while (1)
    {
        DUMP_PID;
        if (logs) printf ("Waiting for the request..\n");

        /** Waiting for the request from host **/
        if (!sig1_flag && !sig2_flag && !term_flag)
            pause();
    
        /** If a message was requested **/
        if (sig1_flag)
        {
            sig1_flag = 0;

            DUMP_PID;
            if (logs) printf ("Sending message to the father. src = %d, dest = %d\n", msg.src, msg.dest);

            /** Sending message **/
            assert (write (gates[1], &msg, sizeof (struct message)) != -1);
            
            /** Sending answer signal **/
            assert (kill (father_pid, SIGUSR1) != -1);
        }
        /** If preparation to the transaction requested **/ 
        else if (sig2_flag)
        {
            struct message recieved_msg;
            int i = 0;
            sig2_flag = 0;

            DUMP_PID;
            if (logs) printf ("Recieving message from father.\n");

            /** Recieving message **/
            assert (read (gates[0], &recieved_msg, sizeof (struct message)) != -1);
            
            /** printing message **/
            printf ("message from %d\n%s", recieved_msg.src, recieved_msg.str);

            /** Sending transaction confirmation signal **/
            assert (kill (father_pid, SIGUSR1) != -1);
        } 
        /** If recieved SIGTERM from father **/ 
        else if (term_flag)
        {
            DUMP_PID;
            if (logs) printf ("Terminating..\n");

            /** Exiting client run **/
            return 0;
        } 
    } 
} 
