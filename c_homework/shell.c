#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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
    pid_t child1_pid = -1, child2_pid = -1;
    int pipes[2] = {0, 0};    
    char * next_line = line;

    /** Searching for '|' character **/
    next_line = strchr (line, '|');

    /** If it's not found **/
    if (!next_line)
    {
        /** Executing command **/
        if (!system (line))
            return true;

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
