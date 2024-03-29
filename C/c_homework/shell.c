#include <stdio.h>                          /* SHELL: PIPELINE */
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* macro serves as protection of UNIX system calls and handlers for most of their errors */
#define __syscall__(call)  if ((call) == -1) { fprintf (stderr, "'%s' while calling '%s' in function '%s'.\nLine #%d in file '%s'\n", strerror(errno), #call, __PRETTY_FUNCTION__, __LINE__, __FILE__); exit(errno); }

/* just macro for additional protection of bash function */
#define __shell__(cmd)   __syscall__(bash(cmd))

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

enum redirect_mode
{
    R_WRITE,
    R_READ,
    R_APPEND,
    R_ERROR
};

struct stopped_proc_info
{
    pid_t pid;
    char argv[256];
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

pid_t running_child = 0;
char *running_child_argv = "";
bool sig_flag = false;
int last_signal = 0;
struct stopped_proc_info stopped_proc[1000];
size_t stopped_proc_size = 0;
int pid_pipes[2];
int last_changed_i = -1;
int father_pid = 0;

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief main interpreter
 * @param line command line or pipe to interpret
 * @return on error returns false, otherwise true
 */
bool run_pipeline (char *line);

/**
 * @brief execute command line string
 *
 *  my little addition to UNIX's exec() function family
 *
 * @param script string with special format: filename [arg1] [arg2] ...
 * @return -1 if an error occurred
 */
int execmd (char *script);

/**
 * @brief run shell command in a separate run_pipeline implemented with fork-exec
 *
 * @param command command to execute
 * @return -1 uf an error occurred
 */
int bash (char *command);

/**
 * @brief check if command is "exit"
 *
 * @param str command to check
 * @return true/false
 */
bool iscommand (const char *str, const char *command);

/**
 * @brief run custom commands
 *
 * @param cmd command to intercept
 * @return true if command was run
 */
bool override_cmd (const char *cmd);

/**
 * @brief run redirection command
 *
 * @param line command with redirection
 * @return true or false
 */
bool run_redirection (char *line);

/**
 * @brief signal handler
 *
 * @param sig signal
 */
void sig_handler (int sig);

void usr_handler (int sig);

void father_usr_handler (int sig);

void int_handler (int sig);

char get_proc_state (pid_t pid);

void check_changed_processes ()
{
    int i = 0;

    for (i; i < stopped_proc_size; i++)
    {
        if (get_proc_state (stopped_proc[i].pid) != 'T')
            last_changed_i = i;
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int main (int argc, char **argv)
{
    /* string to store command line and it's params */
    char *line = NULL;
    size_t line_length, line_size = 0;

    signal (SIGUSR1, father_usr_handler);
    __syscall__(pipe (pid_pipes));

    father_pid = getpid();

    /* main cycle */
    while (true)
    {
        /* prompt welcome character */
        check_changed_processes ();
        printf ("❯ ");

        signal (SIGTSTP, int_handler);
        signal (SIGINT, int_handler);

        /* get commands from stdin */
        __syscall__ (line_length = getline (&line, &line_size, stdin));

        signal (SIGTSTP, sig_handler);
        signal (SIGINT, sig_handler);

        /* skip if there is no info rather than \n */
        if (line_length == 1)
            continue;

        /* erase \n - we don't really need it */
        line[line_length - 1] = '\0';

        /* exit if user wants to */
        if (iscommand (line, "exit"))
            break;

        /* emulate entered commands **/
        run_pipeline (line);


    }

    /* free memory */
    free (line);

    return 0;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

bool run_pipeline (char *line)
{
    int pipes[2];                             /* anonymous channel descriptors                        */
    char *next_line = line;                   /* second pointer to the data we want to run_pipeline   */
    pid_t elder_son = -1, younger_son = -1;   /* we need just two brothers to construct this pipeline */


    /* search for the first entry of separator */
    next_line = strchr (line, '|');

    /* skip unnecessary garbage */
    while (isspace (*(line)))
        line++;

    /* if we are at the end of our pipeline */
    if (!next_line)
    {
        /* check if command is empty */
        if (strlen (line) == 0)
        {
            printf ("error: empty command\n");
            return false;
        }

        /* run custom command if exists */
        if (override_cmd (line))
            return true;

        /* execute command */
        run_redirection (line);

        /* exit from processor */
        return true;
    }

    /* replace '|' -> '\0' to separate our commands */
    *next_line = '\0';
    next_line++;

    /* check if command is empty */
    if (strlen (line) == 0)
    {
        printf ("error: empty command\n");
        return false;
    }

    /* create anonymous channel and fork */
    __syscall__ (pipe (pipes));
    __syscall__ (running_child = elder_son = fork ());

    if (!elder_son) /* son's job */
    {
        /*                   redirect stdout to writable end of our pipe               */
        __syscall__ (dup2 (pipes[1], STDOUT_FILENO));
        __syscall__ (close (pipes[1]));                        /* and close it */
        __syscall__ (close (pipes[0]));

        /* run custom command if exists */
        if (override_cmd (line))
            exit (0);

        /* executing command */
        run_redirection (line);

        /* and now he's bored too */
        exit (0);
    }

    /* very familiar trick innit? seems like someone is emulating his elder brother... */
    __syscall__ (close (pipes[1]));                          /* close unused fd */
    __syscall__ (running_child = younger_son = fork ());

    if (!younger_son) /* now youngling's turn */
    {
        /* redirect stdin to pipe and close unused descriptors */
        __syscall__ (dup2 (pipes[0], STDIN_FILENO))
        __syscall__ (close (pipes[0]));

        /* continue executing pipe */
        run_pipeline (next_line);

        /* son's bored */
        exit (0);
    }

    /* finally, father can close last end of the pipe */
    __syscall__ (close (pipes[0]));

    /* oh, dear! seems like we've forgotten someone, shall we wait for them? */
    __syscall__ (waitpid (younger_son, NULL, 0));
    __syscall__ (waitpid (elder_son, NULL, 0));


    /* report to the authorities */
    return true;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int execmd (char *script)
{
    int i = 0;
    int status = -1;                                          /* return value storage              */
    char *cmd = (char *) malloc (strlen (script) + 1);    /* copy of argument string to modify */
    char **args = (char **) malloc (sizeof (char *));         /* null-terminated string array      */
    size_t args_size = 1, cmd_size = 0;                       /* array size's variables            */
    size_t arg_index = 0, cmd_index = 0;                      /* and some indices to handle stuff  */

    /* pointer check */
    assert (args);
    assert (cmd);

    /* calculate amount of memory we need to copy */
    __syscall__ (cmd_size = (strlen (script) + 1));

    /* allocate memory for our copy */
    cmd = (char *) malloc (cmd_size);
    assert (cmd); /* integrity check*/

    /* calling dude from that old copy center */
    strcpy (cmd, script);

    /* parse cycle */
    for (; cmd_index < cmd_size; cmd_index++, arg_index++)
    {
        /* reallocating memory for new memeber */
        args = (char **) realloc (args, ++args_size * sizeof (char *));
        assert (args); /* integrity check */

        /* skip unused characters */
        while (cmd[cmd_index] == ' ' || cmd[cmd_index] == '\t' || cmd[cmd_index] == '\n')
            cmd_index++;

        /* if we face end of the line */
        if (cmd[cmd_index] == '\0')
        {
            free (args[arg_index]); /* free some prisoners and leave */
            break;
        }

        /* set this pointer (cmd + cmd_index) as a start of parameter string */
        args[arg_index] = cmd + cmd_index;

        /* skip to the end of param */
        while (cmd[cmd_index] != ' ' && cmd[cmd_index] != '\t' && cmd[cmd_index] != '\n' && cmd[cmd_index] != '\0')
            cmd_index++;

        /* and terminate it with NULL so execvp could handle it */
        cmd[cmd_index] = '\0';
    }

    /* and finally terminate array itself */
    args[arg_index] = (char *) NULL;

    /* calling our proxy */
    status = execvp (args[0], args);

    /* (i don't really know if we can get here anyway...) */

    free (args);
    free (cmd);

    return status; /* but at least i'll try */
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int bash (char *command)
{
    pid_t pid = -1; /* id of our little newborn process */


    /* childbirth */
    __syscall__ (running_child = pid = fork ());


    sig_flag = false;

    /* if we identify ourselves as a father */
    if (pid > 0)
    {
        int status = -1; /* variable to store children exit code */
        running_child_argv = command;

        /* wait for our newborn to die peacefully   */
        waitpid (pid, &status, WUNTRACED);

        if (sig_flag)
        {
            if (last_signal == SIGTSTP)
            {
                struct stopped_proc_info procinfo = {pid, ""};
                strcpy (procinfo.argv, running_child_argv);

                write (pid_pipes[1], &procinfo, sizeof (struct stopped_proc_info));
                kill (father_pid, SIGUSR1);
            }

            fprintf (stderr, "shell: process with pid %d and argv %s received %s\n", pid, running_child_argv,
                     last_signal == SIGINT ? "SIGINT" : "SIGTSTP");
        }

        running_child = 0;

        return status; /* and report if immediately */
    }
    else /* if we woke up in a newborn kamikadze terrorist baby-body... */
    {

        int status = -1; /* something wrong, I can feel it   */

        /* what a pity, he replaced himself with an impostor */
        status = execmd (command);

        fprintf (stderr, "shell: %s: %s\n", command, strerror (errno));

        exit (0);
    }
}

bool iscommand (const char *str, const char *command)
{
    /* skipping spaces */
    while (isspace (*str))
        str++;

    /* check if first word is command */
    return (strstr (str, command) == str);
}


bool override_cmd (const char *cmd)
{
    const char *arg_str = cmd; /* argument string */


    /* pointer check */
    assert (cmd);

    /* skip cmd name and spacing */
    while (!isspace (*arg_str) && *arg_str != '\0')
        arg_str++;
    while (isspace (*arg_str) && *arg_str != '\0')
        arg_str++;


    /* if command is cd */
    if (iscommand (cmd, "cd"))
    {
        DIR *current_dir = NULL;

        if (strlen (arg_str) == 0)
        {
            __syscall__ (chdir (getenv ("HOME")));
        }
        else
        {
            __syscall__ (chdir (arg_str));
        }

        return true;
    }

    /* if command is pwd */
    if (iscommand (cmd, "pwd"))
    {
        char buffer[256] = "";

        /* printing current working directory */
        printf ("%s\n", getcwd (buffer, 256));

        return true;
    }

    return false;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

bool run_redirection (char *line)
{
    char *filename = NULL, *strtok_output = NULL;
    enum redirect_mode mode = -1;
    int pipes[2];
    int fd = -1;
    int son_pid;
    char *index = 0;

    assert (line);

    strtok_output = line;

    while (*strtok_output != '\0')
    {
        filename = strtok_output;

        strtok_output++;

        while (*strtok_output != '<' && *strtok_output != '>' && *strtok_output != '\0')
            strtok_output++;
    }

    if (filename == line || filename == line + 1)
    {
        /* check if command is empty */
        if (strlen (line) == 0)
        {
            printf ("error: empty command\n");
            return false;
        }

        /* run custom command if exists */
        if (override_cmd (line))
            return true;

        /* execute command */
        __shell__ (line);

        /* exit from processor */
        return true;
    }

    if (*filename == '>')
    {
        if (*(filename - 1) == '>')
            mode = R_APPEND;
        else if (*(filename - 1) == '2')
            mode = R_ERROR;
        else
            mode = R_WRITE;
    }
    else
        mode = R_READ;


    *filename = '\0';
    if (mode == R_APPEND || mode == R_ERROR)
        *(filename - 1) = '\0';
    filename++;
    while (isspace (*filename))
        filename++;

    index = filename;
    while (!isspace (*index) && *index != '\0')
        index++;

    *index = '\0';

    if (!strlen (filename))
    {
        fprintf (stderr, "shell: error: invalid filename\n");
        return false;
    }

    switch (mode)
    {
        case R_READ:
            fd = open (filename, O_RDONLY);
            if (fd == -1)
            {
                fprintf (stderr, "shell: error while opening file: %s", strerror (errno));
                return false;
            }
            break;

        case R_WRITE:
        case R_ERROR:
            if ((fd = open (filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
            {
                fprintf (stderr, "shell: error: '%s' while opening file '%s'\n", strerror (errno), filename);
                return false;
            }
            break;

        case R_APPEND:
            if ((fd = open (filename, O_WRONLY | O_CREAT | O_APPEND, 0664)) == -1)
            {
                fprintf (stderr, "shell: error: '%s' while opening file '%s'\n", strerror (errno), filename);
                return false;
            }
            break;

        default:
            break;
    }

    __syscall__ (running_child = son_pid = fork ());

    if (!son_pid)
    {
        switch (mode)
        {
            case R_READ:
                __syscall__ (dup2 (fd, STDIN_FILENO));
                break;

            case R_WRITE:
            case R_APPEND:
                __syscall__ (dup2 (fd, STDOUT_FILENO));
                break;

            case R_ERROR:
                __syscall__ (dup2 (fd, STDERR_FILENO));
                break;

            default:
                break;
        }
        __syscall__ (close (fd));

        run_redirection (line);

        exit (0);
    }

    __syscall__ (close (fd));
    __syscall__ (waitpid (son_pid, NULL, 0));

    return true;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void sig_handler (int sig)
{
    if (!running_child)
        return;

    sig_flag = true;
    last_signal = sig;

    kill (running_child, sig);

}

void father_usr_handler (int sig)
{
    struct stopped_proc_info procinfo = {0, ""};
    int i = 0;
    read (pid_pipes[0], &procinfo, sizeof (struct stopped_proc_info));

    for (; i < stopped_proc_size; i++)
    {
        if (stopped_proc[i].pid == 0)
        {
            stopped_proc[i] = procinfo;
            return;
        }
    }

    stopped_proc[stopped_proc_size++] = procinfo;
}

void int_handler (int sig)
{
    int i = 0;


    for (; i < stopped_proc_size; i++)
    {
        if (stopped_proc[i].pid == 0)
            continue;

        if (get_proc_state (stopped_proc[i].pid) != 'T')
        {
            kill (sig, stopped_proc[i].pid);
            fprintf (stderr, "shell: process with pid %d and argv %s received %s\n",
                     stopped_proc[last_changed_i].pid, stopped_proc[last_changed_i].argv,
                     sig == SIGINT ? "SIGINT" : "SIGTSTP");

            printf ("❯ ");

            if (sig == SIGINT)
                stopped_proc[i].pid = 0;

            last_changed_i = -1;

            return;
        }
    }

    if (last_changed_i == -1)
    {

        if (sig == SIGTSTP)
        {
            signal (SIGTSTP, SIG_DFL);
            raise (SIGTSTP);
            return;
        }
        else
            exit (0);
    }

    fprintf (stderr, "shell: process with pid %d and argv %s received %s\n", stopped_proc[last_changed_i].pid,
             stopped_proc[last_changed_i].argv, sig == SIGINT ? "SIGINT" : "SIGTSTP");
    printf ("❯ ");
    fflush (stdout);

    if (get_proc_state (stopped_proc[last_changed_i].pid) != 'T')
        stopped_proc[last_changed_i].pid = 0;

    last_changed_i = -1;
}

char get_proc_state (pid_t pid)
{
    char buffer[32];
    FILE *stat = NULL;
    char state = 0;

    if (pid == 0)
        return true;

    sprintf (buffer, "/proc/%d/stat", pid);
    stat = fopen (buffer, "r");

    fscanf (stat, "%s %s %c", buffer, buffer, &state);
    fclose (stat);

    return state;

}
