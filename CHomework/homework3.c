//===============================================================================================================
// Includes
//===============================================================================================================

// STD
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <memory.h>

// My libs
#include "colors.h"

//===============================================================================================================
// Function declarations
//===============================================================================================================

// Start of quicksort algorithm
void my_qsort (void * array, unsigned int size, unsigned int member_size, int (*compare_func) (const void *, const void *));

// Recursive part of quicksort algorithm
void qsort_r    (void * array, unsigned int left, unsigned int right, unsigned int member_size, int (*compare_func) (const void *, const void *));

// Partition of the array
unsigned int  partition  (void * array, unsigned int left, unsigned int right, unsigned int member_size, int (*compare_func) (const void *, const void *));

// Swap function
void swap (void * a, void * b, unsigned int member_size);

// Unit tests for my quick sort
int sort_unit_test (unsigned int array_length);

// Compare function for chars (from lower to higher)
int comp_char_low_high (const char * left, const char * right);

// Compare function for chars (from lower to higher)
int comp_char_high_low (const char * left, const char * right);

// Compare function for strings (from lower to higher)
int my_strcmp (const char ** str1, const char ** str2);

// Compare function for strings (from lower to higher)
int my_strcmp_r (const char ** str1, const char ** str2);

// Unit tests runner
int run_tests (unsigned int tests_count);

// Console buffer flush function
void flush_buffer ();

// Cyclic shift to the left
void ROL (char ** array, unsigned int start, unsigned int end);

// Get string from input
char * get_string ();

// Separate string function
void process_string (char * string, char separator, char *** string_array, unsigned int * string_array_size);

// Count length of string
int my_strlen (const char * string);

// Delete strings shorter than N
void delete_shorter (char *** string_array, unsigned int * array_size, unsigned int N);

//===============================================================================================================
// Main
//===============================================================================================================

int main (int argc, char ** argv)
{
    char separator = ' ';                   // string separator
    int command = -1;                       // user command
    char * string = "";                     // string we need to process
    int N = -1;                             // delete count
    char ** string_array = NULL;            // string array
    unsigned int string_array_size = 0;     // string array size
    int i = 0;                              // index

    // command line mode
    if (argc > 2)
    {
        if (sscanf (argv[1], "%c", &separator) != 1)
        {
            set_tmcolor (TM_COLOR_RED);
            printf ("Wrong parameters format.\n");
            return 0;
        }

        if (sscanf (argv[2], "%d", &command) != 1 || !command || command > 3)
        {
            set_tmcolor (TM_COLOR_RED);
            printf ("Wrong parameters format.\n");
            return 0;
        }

        if (argc >= 4)
        {
            if (command != 3 || sscanf (argv[3], "%d", &N) != 1)
            {
                set_tmcolor (TM_COLOR_RED);
                printf ("Wrong parameters format.\n");
                return 0;
            }
        }

        printf ("Enter string: ");
        string = get_string();

        process_string (string, separator, &string_array, &string_array_size);

        switch (command)
        {
            case 1:
                my_qsort (string_array, string_array_size, sizeof (char *),
                          (int (*) (const void *, const void *)) my_strcmp);
                break;

            case 2:
                my_qsort (string_array, string_array_size, sizeof (char *),
                          (int (*) (const void *, const void *)) my_strcmp_r);
                break;

            case 3:
                delete_shorter (&string_array, &string_array_size, N);
                break;

            default:
                break;
        }


        printf ("\n");

        for (i = 0; i < string_array_size; i++)
        {
            printf ("%s\n", string_array[i]);
        }

        printf ("\n");

        for (i = 0; i < string_array_size; i++)
            free (string_array[i]);
        free (string_array);

    }
    else
    {
        printf ("Enter string: ");
        string = get_string();
    }

    while (1)
    {
        while (1)
        {
            int scanf_output = 0;
            printf ("Enter separator character (q for exit): ");

            scanf_output = scanf ("%c", &separator);

            if (scanf_output == EOF)
            {
                set_tmcolor (TM_COLOR_RED);
                printf ("Reached end of input stream, aborting...\n");

                free (string);
                return 0;
            }
            else if (scanf_output == 1)
                break;

            set_tmcolor (TM_COLOR_YELLOW);
            printf ("Wrong format, please, try again.\n");
            reset_tmcolor ();

            flush_buffer();
        }

        if (separator == 'q')
            break;

        while (1)
        {
            int scanf_output = 0;
            printf ("Enter mode.\n\n(1) Sort strings lower to upper\n(2) Sort strings upper to lower\n(3) Delete strings, that shorter than N\n");

            scanf_output = scanf ("%d", &command);

            if (scanf_output == EOF)
            {
                set_tmcolor (TM_COLOR_RED);
                printf ("Reached end of input stream, aborting...\n");
                free (string);
                return 0;
            }
            else if (scanf_output == 1 || !command || command > 3)
                break;

            set_tmcolor (TM_COLOR_YELLOW);
            printf ("Wrong format, please, try again.");
            reset_tmcolor ();

            flush_buffer();
        }

        process_string (string, separator, &string_array, &string_array_size);

        switch (command)
        {
            case 1:
                my_qsort (string_array, string_array_size, sizeof (char *),
                          (int (*) (const void *, const void *)) my_strcmp);
                break;

            case 2:
                my_qsort (string_array, string_array_size, sizeof (char *),
                          (int (*) (const void *, const void *)) my_strcmp_r);
                break;

            case 3:
                while (1)
                {
                    int scanf_output = 0;
                    printf ("Enter  N: ");

                    scanf_output = scanf ("%d", &N);

                    if (scanf_output == EOF)
                    {
                        set_tmcolor (TM_COLOR_RED);
                        printf ("Reached end of input stream, aborting...\n");
                        free (string);
                        return 0;
                    }
                    else if (scanf_output == 1 || !command)
                        break;

                    set_tmcolor (TM_COLOR_YELLOW);
                    printf ("Wrong format, please, try again.");
                    reset_tmcolor ();

                    flush_buffer();
                }

                delete_shorter (&string_array, &string_array_size, N);
                break;

            default:
                break;
        }

        printf ("\n");

        for (i = 0; i < string_array_size; i++)
        {
            printf ("%s\n", string_array[i]);
        }

        printf ("\n");

        flush_buffer();
        for (i = 0; i < string_array_size; i++)
            free (string_array[i]);
        free (string_array);
    }

    free (string);
    return 0;
}

//===============================================================================================================
// Function definitions
//===============================================================================================================

void my_qsort (void * arr, unsigned int size, unsigned int member_size, int (*compare_func) (const void *, const void *))
{
    if (!size)
        return;

    qsort_r (arr, 0, size - 1, member_size, compare_func);
}

//===============================================================================================================

void qsort_r (void * array, unsigned int left, unsigned int right, unsigned int member_size, int (*compare_func) (const void *, const void *))
{
    unsigned int p = 0;

    if (left >= right)
        return;

    p = partition (array, left, right, member_size, compare_func);
    qsort_r (array, left, p, member_size, compare_func);
    qsort_r (array, p + 1, right, member_size, compare_func);
}

//===============================================================================================================

unsigned int partition (void * array, unsigned int left, unsigned int right, unsigned int member_size, int (*compare_func) (const void *, const void *))
{
    // i - left index, j - right index, p - pivot
    unsigned int i = left, j = right;

    void * p = malloc (member_size);
    assert (p != NULL);

    memcpy (p, array + ((left + right) / 2) * member_size, member_size);

    while (1)
    {
        while ((*compare_func) ((void*)((char*)array + i * member_size), p) < 0)
            i++;

        while ((*compare_func) ((void*)((char*)array + j * member_size), p) > 0)
            j--;

        if (i >= j)
        {
            free (p);
            return j;
        }

        swap ((void*)((char*)array + i++ * member_size), (void*)((char*)array + j-- * member_size), member_size);
    }
}

//===============================================================================================================

void swap (void * a, void * b, unsigned  int member_size)
{
    void * temp = malloc (member_size);
    assert (temp != NULL);

    memcpy (temp, a,    member_size);
    memcpy (a,    b,    member_size);
    memcpy (b,    temp, member_size);

    free (temp);
}

//===============================================================================================================

int sort_unit_test (unsigned int array_length)
{
    int i = 0;
    char * array1 = malloc (array_length);
    char * array2 = malloc (array_length);

    assert (array1 != NULL);
    assert (array2 != NULL);

    for (i = 0; i < array_length; i++)
    {
        array1[i] = array2[i] = (char) (rand () % 256);
    }

    my_qsort (array1, array_length, sizeof (char), (int (*) (const void *, const void *)) comp_char_low_high);
    qsort (array2, array_length, sizeof (char), (int (*) (const void *, const void *)) comp_char_low_high);

    for (i = 0; i < array_length; i++)
    {
        if (array1[i] != array2[i])
        {
            printf ("\nSort test failed. Arrays dump:\n");
            printf ("array1: ");

            for (i = 0; i < array_length; i++)
                printf ("%3d ", array1[i]);

            printf ("\narray2: ");

            for (i = 0; i < array_length; i++)
                printf ("%3d ", array2[i]);

            printf ("\n");

            free (array1);
            free (array2);

            return 0;
        }
    }

    free (array1);
    free (array2);
    return 1;
}

//===============================================================================================================

int comp_char_low_high (const char * left, const char * right)
{
    return *left - *right;
}

//===============================================================================================================

int comp_char_high_low (const char * left, const char * right)
{
    return *right - *left;
}

//===============================================================================================================

int run_tests (unsigned int tests_count)
{
    int result = 1;
    unsigned int i;

    srand (time(NULL));

    for (i = 0; i < tests_count; i++)
    {
        unsigned int array_size = rand() % 20;

        printf ("Test #%4u. array_size = %4u. ", i, array_size);

        if (sort_unit_test (array_size))
        {
            printf ("[");
            $fgr        printf ("PASSED");
            $def        printf ("]\n");
        }
        else
        {
            printf ("[");
            $frd        printf ("FAILED");
            $def        printf ("]\n");

            result = 0;
        }
    }

    return result;
}

//===============================================================================================================

void flush_buffer ()
{
    int c;

    while ((c = getchar()) != EOF && c != '\n');
}

//===============================================================================================================

void ROL (char ** array, unsigned int start, unsigned int end)
{
    unsigned int i = 0;
    assert (start <= end);
    char * buffer = array[start];

    for (i = start; i < end; i++)
        array[i] = array[i + 1];

    array[end] = buffer;
}

//===============================================================================================================

char * get_string ()
{
    int c = 0;                                      // character
    unsigned int string_size = 0, alloc_size = 1;   // string size and allocation size
    char * string = malloc (alloc_size);            //

    assert (string != NULL);

    while (scanf ("%c", &c) == 1)
    {
        if (c == '\n' || c == EOF)
            break;

        string_size++;
        if (string_size > alloc_size)
        {
            alloc_size *= 2;
            string = realloc (string, alloc_size);
            assert (string != NULL);
        }

        string [string_size - 1] = (char) c;
    }

    if (alloc_size > string_size + 1)
        string = realloc (string, string_size + 1);

    assert (string != NULL);

    string [string_size] = '\0';

    return string;
}

//===============================================================================================================

void process_string (char * string, char separator, char *** string_array, unsigned int * string_array_size)
{
    *string_array_size = 0;
    int i = 0;

    while (string[i] != '\0')
    {
        unsigned int string_size = 0;
        (*string_array_size)++;

        if (i == 0)
            *string_array = malloc (sizeof (char *));
        else
            *string_array = realloc (*string_array, *string_array_size * sizeof (char*));

        assert (*string_array != NULL);

        while (1)
        {
            string_size++;

            if (string_size == 1)
                (*string_array)[*string_array_size - 1] = malloc (sizeof (char*));
            else
                (*string_array)[*string_array_size - 1] = realloc ((*string_array)[*string_array_size - 1], sizeof(char) * string_size);

            assert (*string_array != NULL);

            if (string[i] == separator || string[i] == '\0')
            {
                (*string_array)[*string_array_size - 1][string_size - 1] = '\0';
                i++;
                break;
            }

            (*string_array)[*string_array_size - 1][string_size - 1] = string[i];
            i++;
        }

    }

    delete_shorter (string_array, string_array_size, 1);
}

//===============================================================================================================

int my_strcmp (const char ** str1, const char ** str2)
{
    int i = 0;

    while ((*str1)[i] != '\0' || (*str2)[i] != '\0')
    {
        if ((*str2)[i] == '\0')
            return 1;

        if ((*str1)[i] == '\0')
            return -1;

        if ((*str1)[i] - (*str2)[i] != 0)
            return (*str1)[i] - (*str2)[i];

        i++;
    }

    return 0;
}

//===============================================================================================================

int my_strcmp_r (const char ** str1, const char ** str2)
{
    return -my_strcmp (str1, str2);
}

//===============================================================================================================

int my_strlen (const char * string)
{
    int i = 0;

    for (i = 0; string[i] != '\0'; i++)
    {
    }

    return i;
}

//===============================================================================================================

void delete_shorter (char *** string_array, unsigned int * array_size, unsigned int N)
{
    unsigned int size = *array_size;  // local copy of array size
    int i = 0;                        // index

    while (i < size)
    {
        if (my_strlen ((*string_array)[i]) < N)
        {
            free ((*string_array)[i]);
            ROL (*string_array, i, size - 1);
            size--;
        }
        else
            i++;
    }

    *string_array = realloc (*string_array, sizeof (char*) * size);
    assert (*string_array != NULL);

    *array_size = size;
}