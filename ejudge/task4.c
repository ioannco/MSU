#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Node
{
    struct Node *prev, *next;
    char *elem;
};

struct List
{
    struct Node *first, *last;
};

#define STRING_COUNT 3

void process (struct List *pl, const char *str);

int main ()
{
    char *strs[STRING_COUNT] = {"f", "e", "d"};
    struct List *pl = malloc (sizeof (struct Node));
    int i = 0;
    struct Node * current = NULL;

    pl->first = (struct Node *) malloc (sizeof (struct Node));
    pl->first->prev = pl->first->next = NULL;

    current = pl->first;
    current->elem = (char *) malloc (strlen (strs[0]) + 1);
    strcpy (current->elem, strs[0]);

    for (i = 1; i < STRING_COUNT; i++)
    {
        current->next = (struct Node *) malloc (sizeof (struct Node));
        current->next->prev = current;
        current->next->next = NULL;

        current->next->elem = (char *) malloc (strlen (strs[i]) + 1);
        strcpy (current->next->elem, strs[i]);

        current = current->next;
    }

    pl->last = current;

    printf ("list before process: \n");

    for (current = pl->first; current != NULL; current = current->next)
    {
        printf("%s\n", current->elem);
    }
    
    process (pl, "b");

    printf ("list after: \n");

    for (current = pl->first; current != NULL; current = current->next)
    {
        printf("%s\n", current->elem);

        printf ("el->prev = %X, el->next = %X\n", current->prev, current->next);
    }

    free(pl);
}

void process (struct List *pl, const char *str)
{
    if (!pl || !pl->first || !pl->last)
        return;

    struct Node * current = pl->first;
    struct Node * end = pl->last;

    if (current == end)
        return;

    do
    {
        if (strcmp (current->elem, str) == 0)
        {
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
            current = current->next;

    } while (current && current != end);
}

