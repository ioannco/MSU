#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct FileNode
{
    FILE * file;
    int num;
    struct FileNode * prev, * next;
};

struct FileNode * min_node (struct FileNode * head);
void delete_node (struct FileNode * victim);
void add_node (struct FileNode * head, FILE * file, int num);

int main (int argc, char ** argv)
{
    int i = 0;
    struct FileNode * files_list = (struct FileNode *) malloc (sizeof (struct FileNode));

    if (argc < 2)
        return 0;

    files_list->prev = files_list->next = NULL;
    files_list->num = 0;
    files_list->file = NULL;

    for (; i < argc - 1; i++)
    {
        FILE * file = fopen(argv[i + 1], "r");
        int num = 0;

        assert (file != NULL);

        if (fscanf (file, "%d", &num) != 1)
        {
            fclose (file);
            continue;
        }

        add_node (files_list, file, num);
    }

    while (files_list->next)
    {
        struct FileNode * min = min_node (files_list);
        printf ("%d ", min->num);
        if (fscanf (min->file, "%d", &(min->num)) != 1)
            delete_node (min);
    }

    printf ("\n");

    return 0;
}

struct FileNode *min_node (struct FileNode *head)
{
    assert (head);

    struct FileNode * current = head->next, * min = head->next;


    for (; current != NULL; current = current->next)
    {
        if (current->num < min->num)
            min = current;
    }

    return min;
}

void delete_node (struct FileNode *victim)
{
    assert (victim);

    if (victim->prev)
        victim->prev->next = victim->next;
    if (victim->next)
        victim->next->prev = victim->prev;

    if (victim->file)
        fclose (victim->file);

    free (victim);
}

void add_node (struct FileNode *head, FILE *file, int num)
{
    assert (head);

    struct FileNode * new = (struct FileNode *) malloc (sizeof (struct FileNode));

    new->prev = head;
    new->next = head->next;

    if (head->next) head->next->prev = new;
    head->next = new;

    new->file = file;
    new->num = num;
}
