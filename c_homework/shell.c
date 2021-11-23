#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

bool process (char * line);

int main (int argc, char ** argv)
{
    /** Line and it's parameters. line_length - count of characters in line, line_size - size of the line in bytes **/
    char * line = NULL;
    size_t line_length = 0, line_size = 0;

    /** Main cycle **/
    while (true)
    {
        /** Entering start symbol **/
        printf ("> ");

        /** Getting line from STDIN **/
        line_length = getline (&line, &line_size, stdin);

        if (line_length == -1)
        {
            printf ("error: STDIN reached end of input\n");
            break;
        }

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

bool process (char * line)
{
    int pipes[2] = {0, 0};
    char * next_line = line;

    /** Searching for '|' character **/
    next_line = strchr (line, '|');

    /** Skipping delimiters **/
    while (*line == ' ' || *line == '\t')
        (*line)++;

    /** If it's not found **/
    if (!next_line)
    {
        pid_t child_pid = fork();

        if (child_pid > 0)
        {
            int i = 0;
            const char ** args = NULL;
            

            for (i = 0; i)

            execlp ("")
        }
        /** Executing command **/


        return false;
    } 

    next_line++;

    if (*next_line == '|')
    {
        printf ("error: '||' is not allowed.\n");
        return false;        
    }
    
    return true;
}
