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
    char *strs[STRING_COUNT] = {"c", "b", "a    "};
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

    process (pl, "b");

    for (current = pl->first; current != NULL; current = current->next)
    {
        printf("%s\n", current->elem);
    }

    free(pl);
}

#include "task4_only_function.h"