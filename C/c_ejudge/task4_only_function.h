#include <string.h>
#include <stdlib.h>

void process (struct List *pl, const char *str)
{
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


            struct Node * next = current->next;

            current->prev = pl->last;
            current->next = NULL;

            pl->last->next = current;

            current = next;
        }
        else
            current = current->next;

    } while (current && current != end->next);
}