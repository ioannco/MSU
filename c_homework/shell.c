#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef LOGS
#define LOGS 1
#endif

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <execinfo.h>

#define __sys_call(call)  if ((call) == -1) { fprintf (stderr, "'%s' while calling '%s' in function '%s'.\nLine #%d in file '%s'\n", strerror(errno), #call, __PRETTY_FUNCTION__, __LINE__, __FILE__); exit(errno); }
#define __shell(cmd)            __sys_call(bash(cmd))

bool process (char *line);
int execcl (char *command);
int bash (char *command);

int main ()
{
    /** Line and it's parameters. line_length - count of characters in line, line_size - size of the line in bytes **/
    char *line = NULL;
    size_t line_length, line_size = 0;

    /** Main cycle **/
    while (true)
    {
        /** Entering start symbol **/
        printf ("❯ ");

        /** Getting line from STDIN **/
        __sys_call (line_length = getline (&line, &line_size, stdin));

        /** Skipping if line is empty (only \n) **/
        if (line_length == 1)
            continue;

        /** Deliting \n **/
        line[line_length - 1] = '\0';

        /** Exiting if 'exit' was entered **/
        if (!strcmp (line, "exit"))
            break;

        /** Processing input. If an error occured, exiting **/
        process (line);
    }

    free (line);
}

bool process (char *line)
{
    int pipes[2] = {0, 0};
    char *next_line = line;
    pid_t son1 = -1, son2 = -1;

    /** Searching for '|' character **/
    next_line = strchr (line, '|');

    /** Skipping delimiters **/
    while (*line == ' ' || *line == '\t')
        line++;

    /** If it's not found **/
    if (!next_line)
    {

        /** Executing command **/
        __shell (line);
        return true;
    }

    *next_line = '\0';
    next_line++;

    if (*next_line == '|')
    {
        printf ("error: '||' is not allowed.\n");
        return false;
    }

    __sys_call (pipe (pipes));
    __sys_call (son1 = fork ());

    if (!son1)
    {
        __sys_call (dup2 (pipes[0], STDIN_FILENO))
        __sys_call (close (pipes[0]));;
        __sys_call (close (pipes[1]));

        process (next_line);

        exit (0);
    }

    __sys_call (close(pipes[0]));
    __sys_call (son2 = fork());

    if (!son2)
    {
        __sys_call (dup2 (pipes[1], STDOUT_FILENO));
        __sys_call (close (pipes[1]));

        __shell (line);

        exit(0);
    }

    __sys_call (close (pipes[1]));

    __sys_call (waitpid(son2, NULL, 0));
    __sys_call (waitpid(son1, NULL, 0));

    return true;
}

int execcl (char *command)
{
    int status = -1;
    char *cmd = (char *) malloc (strlen (command) + 1);
    char **args = (char **) malloc (sizeof (char *));
    size_t args_size = 1, cmd_size = 0;
    size_t arg_index = 0, cmd_index = 0;

    assert (args);
    assert (cmd);

    __sys_call (cmd_size = (strlen (command) + 1));

    cmd = (char *) malloc (cmd_size);
    assert (cmd);

    strcpy (cmd, command);

    for (; cmd_index < cmd_size; cmd_index++, arg_index++)
    {
        args = (char **) realloc (args, ++args_size * sizeof (char *));
        assert (args);

        while (cmd[cmd_index] == ' ' || cmd[cmd_index] == '\t' || cmd[cmd_index] == '\n')
            cmd_index++;

        if (cmd[cmd_index] == '\0')
        {
            free (args[arg_index]);
            break;
        }

        args[arg_index] = cmd + cmd_index;

        while (cmd[cmd_index] != ' ' && cmd[cmd_index] != '\t' && cmd[cmd_index] != '\n' && cmd[cmd_index] != '\0')
            cmd_index++;

        cmd[cmd_index] = '\0';
    }

    args[arg_index] = (char *) NULL;



    __sys_call (status = execvp (args[0], args));

    free (cmd);
    return status;
}

int bash (char *command)
{
    pid_t pid = -1;

    __sys_call (pid = fork ());

    if (pid > 0)
    {
        int status = -1;

        __sys_call (wait (&status));
        return status;
    }
    else
    {
        int status = -1;

        __sys_call (status = execcl (command));
        return status;
    }
}


