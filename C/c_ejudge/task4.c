#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 16

struct Node
{
    struct Node *prev, *next;
    char *elem;
};

struct List
{
    struct Node *first, *last;
};

char ** get_strings (unsigned int * strings_count);
void print_list (struct List * pl);
void process (struct List *pl, const char *str);

int main ()
{
    unsigned int strings_count = 0;
    char ** strs = get_strings (&strings_count);
    struct List *pl = malloc (sizeof (struct Node));
    int i = 0;
    struct Node * current = NULL;
    char control_str[16] = "";

    pl->first = (struct Node *) malloc (sizeof (struct Node));
    pl->first->prev = pl->first->next = NULL;

    current = pl->first;
    current->elem = (char *) malloc (strlen (strs[0]) + 1);
    strcpy (current->elem, strs[0]);

    for (i = 1; i < strings_count; i++)
    {
        current->next = (struct Node *) malloc (sizeof (struct Node));
        current->next->prev = current;
        current->next->next = NULL;

        current->next->elem = (char *) malloc (strlen (strs[i]) + 1);
        strcpy (current->next->elem, strs[i]);

        current = current->next;
    }

    pl->last = current;

    printf ("enter control string: ");
    scanf ("%s", control_str);

    printf ("list before process: \n");

    print_list (pl);
    
    process (pl, control_str);

    printf ("\nlist after: \n");

    print_list (pl);

    for (current = pl->first; current != NULL;)
    {
        struct Node * deletant = current;
        current = current->next;

        free (deletant);
        deletant = NULL;
    }

    free(pl);
    pl = NULL;

    for (i = 0; i < strings_count; i++)
    {
        free (strs[i]);
        strs[i] = NULL;
    }

    free (strs);
    strs = NULL;

    return 0;
}

char ** get_strings (unsigned int * strings_count)
{
    unsigned int i = 0;
    char ** strings = NULL;

    printf ("enter strings count: ");
    scanf ("%u", strings_count);

    strings = (char **) malloc (sizeof (char *) * (*strings_count));

    printf ("enter strings:\n\n");

    for (; i < *strings_count; i++)
    {
        printf ("> ");

        strings[i] = (char *) malloc (BUFFER_SIZE);
        scanf ("%s", strings[i]);
    }

    printf ("\nstring input completed!\n");

    return strings;
}

void print_list (struct List * pl)
{
    struct Node * current = pl->first;

    for (current = pl->first; current != NULL; current = current->next)
    {
        printf("'%s'  ", current->elem);
        printf ("(el = [0x%X], el->prev = [0x%X], el->next = [0x%X])\n", current, current->prev, current->next);
    }
}

void process (struct List *pl, const char *str)
{
    struct Node * current = pl->first;
    struct Node * end = pl->last;
    struct Node * last_processed = NULL;

    printf ("\n[process]: started processing. Control string is '%s'.\n", str);

    if (!pl || !pl->first || !pl->last)
    {
        printf ("[process]: null pointer found.\n");
        return;
    }

    if (current == end)
    {
        printf ("[process]: current == end.\n");
        return;
    }

    do
    {
        printf ("[process]: current->elem = '%s', current = [0x%X], current->prev = [0x%X], current->next = [0x%X], end = [0x%X]\n", current->elem, current, current->prev, current->next, end);
        last_processed = current;

        if (strcmp (current->elem, str) == 0)
        {
            printf ("[process]: current node's string is equal to control string, deleting node...\n");

            if (current->prev)
                current->prev->next = current->next;

            if (current->next)
                current->next->prev = current->prev;

            if (pl->first == current)
					pl->first = current->next;
			if (pl->last == current)
					pl->last = current->prev;
           
		   	struct Node * next = current->next;

            free (current->elem);
            current->elem = NULL;

            free (current);

            current = next;
                    
        }
        else if (strcmp (current->elem, str) > 0)
        {
            printf ("[process]: current node's string is larger than control string, moving node to the end of list...\n");

            if (!current->next)
                break;

            current->next->prev = current->prev;

            if (current->prev)
                current->prev->next = current->next;
            else
                current->next->prev = NULL;

            struct Node * next = current->next;

            current->prev = pl->last;
            current->next = NULL;

            pl->last->next = current;
			pl->last = current;

            if (pl->first == current)
                pl->first = next;

            current = next;
        }
        else
        {
            printf ("[process]: current node's string is smaller than control string, skipping node...\n");
            current = current->next;
        }

        printf ("[process]: list after iteration:\n");
        print_list (pl);
        printf ("[process]: iteration completed. current->prev = [0x%X], end = [0x%X]\n\n", current->prev, end);

    } while (current && last_processed != end);

    printf ("[process]: process completed\n");
}

