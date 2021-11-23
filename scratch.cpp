/*
* gcc -Wall -Wextra -pedantic -Werror -o piper piper.c
*/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/wait.h>

int processor (char *line)
{
    pid_t pid, pid2; // Forked prcesses pids
    int fds[2]; // Pipe file descriptors
    char *next = strchr (line, '|'); // Find next '|' if any

// If '|' is found
    if (next)
    {
        *next = '\0'; // Break the line by '|'
        next++; // Update next to next line
    }

// Trim front spaces and tabs
    while ((*line == ' ') || (*line == '\t'))
        line++;

// If line is empty then show error message and return
    if (*line == '\0')
    {
        printf ("Error: command is omited!\n");
        return -1;
    }

// If this line is the last then execute it and return
    if (!next)
        return system (line);

// Create pipe file descriptots to communicate two processes
    if (pipe (fds) < 0)
    {
// Show error message and return if error occurs
        perror ("pipe()");
        return -1;
    }

// Fork to two processes
    if ((pid = fork ()) < 0)
    {
// Show error message and return if error occurs
        perror ("fork()");
        return -1;
    }

// Child process
    if (!pid)
    {
// Use read end of pipe as standard input
        dup2 (fds[0], STDIN_FILENO);
        close (fds[0]); // Close unused fd
        close (fds[1]); // Close unused fd
        processor (next); // Process next line (after '|')
        exit (0); // Exit after processing
    }

// Parent process
    else
    {
        close (fds[0]); // Close unused fd

// Fork to two processes
        if ((pid2 = fork ()) < 0)
        {
            close (fds[1]); // Close unused fd

// Show error message and return if error occurs
            perror ("fork()");
            return -1;
        }

// Parent process
        if (pid2)
        {
            close (fds[1]); // Close unused fd

// Wait second child process
            waitpid (pid2, NULL, 0);

// Wait first child process
            waitpid (pid, NULL, 0);
        }

// Child process
        else
        {
// Use read end of pipe as standard input
            dup2 (fds[1], STDOUT_FILENO);
            close (fds[1]); // Close unused fd
            system (line); // Execute line
            exit (0); // Exit the process
        }
    }

// Return OK error code
    return 0;
}


int main (void)
{
    char *line = NULL; // Entered line
    size_t cap = 0; // Size of line in bytes
    ssize_t len; // Current entered line length

// Hello message
    printf ("Piper v1.0, 2020, Public Domain\n");

// Infinite loop
    while (1)
    {
// Prompt
        printf ("piper> ");

// Enter line, break the loop if input is closed or error
        if ((len = getline (&line, &cap, stdin)) <= 0)
            break;

// Skip empty line
        if (len == 1)
            continue;

// Trim end of line character
        line[len - 1] = '\0';

// Break the loop if exit is entered
        if (!strcmp (line, "exit"))
            break;

// Process entered line
        processor (line);
    }

// Free line buffer
    free (line);

    return 0;
}