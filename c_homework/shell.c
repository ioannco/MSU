#include <stdio.h>                                                         /* SHELL: PIPELINE */
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* macro serves as protection of UNIX system calls and handlers for most of their errors */
#define __system_call__(call)  if ((call) == -1) { fprintf (stderr, "'%s' while calling '%s' in function '%s'.\nLine #%d in file '%s'\n", strerror(errno), #call, __PRETTY_FUNCTION__, __LINE__, __FILE__); exit(errno); }

/* just macro for additional protection of bash function */
#define __shell__(cmd)   __system_call__(bash(cmd))

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

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int main (int argc, char ** argv)
{
    /* string to store command line and it's params */
    char *line = NULL;
    size_t line_length, line_size = 0;

    /* main cycle */
    while (true)
    {
        /* prompt welcome character */
        printf ("❯ ");

        /* get commands from stdin */
        __system_call__ (line_length = getline (&line, &line_size, stdin));

        /* skip if there is no info rather than \n */
        if (line_length == 1)
            continue;

        /* erase \n - we don't really need it */
        line[line_length - 1] = '\0';

        /* exit if user wants to */
        if (!strcmp (line, "exit"))
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
    int pipes[2];                              /* anonymous channel descriptors                        */
    char *next_line = line;                    /* second pointer to the data we want to run_pipeline        */
    pid_t elder_son = -1, younger_son = -1;    /* we need just two brothers to construct this pipeline */

    /* search for the first entry of separator */
    next_line = strchr (line, '|');

    /* skip unnecessary garbage */
    while (*line == ' ' || *line == '\t')
        line++;

    /* if we are at the end of our pipeline */
    if (!next_line)
    {
        /* execute command */
        __shell__ (line);

        /* exit from processor */
        return true;
    }

    /* replace | -> \0 to separate our commands */
    *next_line = '\0';
    next_line++;

    /* check if there are more than one delimiter */
    if (*next_line == '|')
    {
        printf ("error: '||' is not allowed.\n");
        return false;
    }

    /* create anonymous channel and fork */
    __system_call__ (pipe (pipes));
    __system_call__ (elder_son = fork ());

    if (!elder_son) /* son's job */
    {
        /* redirect stdin to pipe and close unused descriptors */
        __system_call__ (dup2 (pipes[0], STDIN_FILENO))
        __system_call__ (close (pipes[0]));;
        __system_call__ (close (pipes[1]));

        /* continue executing pipe */
        run_pipeline (next_line);

        /* son's bored */
        exit (0);
    }

    /* very familiar trick innit? seems like someone is emulating his elder brother... */
    __system_call__ (close(pipes[0]));                          /* close unused fd */
    __system_call__ (younger_son = fork());

    if (!younger_son) /* now youngling's turn */
    {
        /*                   redirect stdout to writable end of our pipe               */
        __system_call__ (dup2 (pipes[1], STDOUT_FILENO));
        __system_call__ (close (pipes[1]));                        /* and close it */

        /* executing command */
        __shell__ (line);

        /* and now he's bored too */
        exit(0);
    }

    /* finally, father can close last end of the pipe */
    __system_call__ (close (pipes[1]));

    /* oh, dear! seems like we've forgotten someone, shall we wait for them? */
    __system_call__ (waitpid(younger_son, NULL, 0));
    __system_call__ (waitpid(elder_son, NULL, 0));

    /* report to the authorities */
    return true;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int execmd (char *script)
{
    int status = -1;                                          /* return value storage              */
    char *cmd = (char *) malloc (strlen (script) + 1);    /* copy of argument string to modify */
    char **args = (char **) malloc (sizeof (char *));         /* null-terminated string array      */
    size_t args_size = 1, cmd_size = 0;                       /* array size's variables            */
    size_t arg_index = 0, cmd_index = 0;                      /* and some indices to handle stuff  */

    /* pointer check */
    assert (args);
    assert (cmd);

    /* calculate amount of memory we need to copy */
    __system_call__ (cmd_size = (strlen (script) + 1));

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
    __system_call__ (status = execvp (args[0], args));

    /* (i don't really know if we can get here anyway...) */

    free (cmd);
    return status; /* but at least i'll try */
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int bash (char *command)
{
    pid_t pid = -1; /* id of our little newborn process */

    /* childbirth */
    __system_call__ (pid = fork ());

    /* if we identify ourselves as a father */
    if (pid > 0)
    {
        int status = -1; /* variable to store children exit code */

        /* wait for our newborn to die peacefully   */
        __system_call__ (wait (&status));
        return status; /* and report if immediately */
    }
    else /* if we woke up in a newborn kamikadze terrorist baby-body... */
    {
        int status = -1; /* something wrong, I can feel it   */

        /* what a pity, he replaced himself with an impostor */
        __system_call__ (status = execmd (command));
        return status; /*          reporting it...           */
    }
}


