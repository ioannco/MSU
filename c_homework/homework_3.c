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

/**
 * @brief Start of the quicksort algorithm
 *
 * @param array         array to sort
 * @param size          size of the array
 * @param member_size   size of array member
 * @param compare_func  pointer to function that compares two members of array
 */
void my_qsort (void * array, unsigned int size, unsigned int member_size, int (*compare_func) (const void *, const void *));

/**
 * @brief Recursive part of quicksort algorithm
 *
 * @param array         array to sort
 * @param left          index to left bound of sort area
 * @param right         index to right bound of sort area
 * @param member_size   size of array member
 * @param compare_func  pointer to function that compares two members of array
 *
 * @see my_qsort (void * array, unsigned int size, unsigned int member_size, int (*compare_func) (const void *, const void *))
 */
void qsort_r (void * array, unsigned int left, unsigned int right, unsigned int member_size, int (*compare_func) (const void *, const void *));

/**
 * @brief Partition of the array
 *
 * @param array         array to part
 * @param left          index to left bound of sort area
 * @param right         index to right bound of sort area
 * @param member_size   size of array member
 * @param compare_func  pointer to function that compares two members of array
 * @return              index of the separator
 * @see                 my_qsort (void * array, unsigned int size, unsigned int member_size, int (*compare_func) (const void *, const void *))
 */
unsigned int  partition  (void * array, unsigned int left, unsigned int right, unsigned int member_size, int (*compare_func) (const void *, const void *));

/**
 * @brief Swap function
 *
 * @param a             pointer to the first element
 * @param b             pointer to the second second element
 * @param member_size   size of elements
 */
void swap (void * a, void * b, unsigned int member_size);

/**
 * @brief Unit tests for my quick sort
 *
 * @param array_length  length of the array you want to test
 * @return              1 if success and 0 if not
 */
int sort_unit_test (unsigned int array_length);

/**
 * @brief Compare function for chars (from lower to higher)
 *
 * @param left  first element
 * @param right second element
 * @return      result of the comparison
 */
int comp_char_low_high (const char * left, const char * right);

/**
 * @brief Compare function for chars (from higher to lower)
 *
 * @param left  first element
 * @param right second element
 * @return      result of the comparison
 */
int comp_char_high_low (const char * left, const char * right);

/**
 * @brief Compare function for strings (from lower to higher)
 *
 * @param left  first element
 * @param right second element
 * @return      result of the comparison
 */
int my_strcmp (const char ** str1, const char ** str2);

/**
 * @brief Compare function for strings (from higher to lower)
 *
 * @param left  first element
 * @param right second element
 * @return      result of the comparison
 */
int my_strcmp_r (const char ** str1, const char ** str2);

//
/**
 * @brief Unit tests runner
 *
 * @param tests_count   Number of tests
 * @return              Success or not
 */
int run_tests (unsigned int tests_count);

/**
 * @brief Console buffer flush function
 */
int flush_buffer ();

/**
 * @brief Cyclic shift to the left
 *
 * @param array     array to shift
 * @param start     start of the shift area
 * @param end       nd of the shift area
 */
void ROL (char ** array, unsigned int start, unsigned int end);

/**
 * @brief Get string from input.
 *
 * Please, release memory of the returned pointer
 *
 * @return pointer to the string
 */
char * get_string ();

/**
 * @brief Separate string function
 *
 * @param string
 * @param separator
 * @param string_array
 * @param string_array_size
 */
void process_string (char * string, char separator, char *** string_array, unsigned int * string_array_size);

/**
 * @brief Count length of string
 *
 * @param string
 * @return elements count
 */
int my_strlen (const char * string);

/**
 * @brief Delete strings shorter than N
 *
 * @param string_array  pointer to the string array
 * @param array_size    string array size
 * @param N
 */
void delete_shorter (char *** string_array, unsigned int * array_size, unsigned int N);


/**
 * @brief Delete strings larger than N
 *
 * @param string_array  pointer to the string array
 * @param array_size    string array size
 * @param N
 */
void delete_larger (char *** string_array, unsigned int * array_size, unsigned int N);

/**
 * @brief Count digits in the number
 *
 * @param number number to count
 * @return digits count
 */
int count_digits (int number);

/**
 * @brief gets char and filter it for EOF or custom condition described in filter
 * @param error_message message that displayed if input filtered
 * @param filter filter function
 * @return input value
 */
char get_char_safe (const char * error_message, int (*filter) (int input));

/**
 * @brief empty filter, always returns 1
 * @param input integer to filter
 * @return 1
 */
int default_filter (int input);

/**
 * @brief filter for my commands
 * @param input integer to filter
 * @return retry or not
 */
int command_filter (int input);

/**
 * @brief filter for N number
 * @param input integer to filter
 * @return retry or not
 */
int N_filter (int input);

/**
 * @brief get integer and filter input
 * @param dest pointer to variable to store input
 * @param error_message retry message
 * @param filter filter
 * @return success
 */
int get_int_safe (int *dest, const char *error_message, int (*filter) (int));

//===============================================================================================================
// Main
//===============================================================================================================

int main (int argc, char ** argv)
{
    char separator = 0;                     // string separator
    int command = -1;                       // user command
    char * string = NULL;                   // string we need to process
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

        if (command == 3 && argc != 4)
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
                delete_larger (&string_array, &string_array_size, N);
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
        printf ("Enter separator character (q to exit): ");
        if (!(separator = get_char_safe ("Wrong format, please, try again.\n", default_filter)))
        {
            free (string);
            return 0;
        }

        if (separator == 'q')
            break;

        printf ("Enter mode.\n\n(1) Sort strings lower to upper\n(2) Sort strings upper to lower\n(3) Delete strings larger than N\n\n> ");
        if (!(command = (int) get_char_safe ("Wrong format, please, try again.\n", command_filter) - '0'))
        {
            free (string);
            return 1;
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
                printf ("Enter N: ");
                if (!(get_int_safe (&N, "Wrong format, please, try again.\n", N_filter)))
                {
                    if (string) free (string);
                    if (string_array)
                    {
                        for (i = 0; i < string_array_size; i++)
                            if (string_array[i])
                            {
                                free (string_array[i]);
                                string_array[i] = NULL;
                            }

                        free (string_array);
                    }
                }

                delete_larger (&string_array, &string_array_size, N);

                break;

            default:
                break;
        }

        printf ("\n");

        for (i = 0; i < string_array_size; i++)
        {
            printf ("#%*.d: %s\n", count_digits ((int) string_array_size), i + 1, string_array[i]);
        }

        printf ("\n");

        if (string_array)
        {
            for (i = 0; i < string_array_size; i++)
                free (string_array[i]);
            free (string_array);
        }
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

    memcpy (p, (void *)((char *) array + ((left + right) / 2) * member_size), member_size);

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
    char * array1 = (char *) malloc (array_length);
    char * array2 = (char *) malloc (array_length);

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

int flush_buffer ()
{
    int c;
    int i = 0;

    while ((c = getchar()) != EOF && c != '\n')
    {
        i++;
    }

    return i;
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
    char * string = (char *) malloc (alloc_size);            //

    assert (string != NULL);

    while (scanf ("%c", &c) == 1)
    {
        if (c == '\n' || c == EOF)
            break;

        string_size++;
        if (string_size > alloc_size)
        {
            alloc_size *= 2;
            string = (char *) realloc (string, alloc_size);
            assert (string != NULL);
        }

        string [string_size - 1] = (char) c;
    }

    if (alloc_size > string_size + 1)
        string = (char *) realloc (string, string_size + 1);

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
            *string_array = (char **) malloc (sizeof (char *));
        else
            *string_array = (char **) realloc (*string_array, *string_array_size * sizeof (char*));

        assert (*string_array != NULL);

        while (1)
        {
            string_size++;

            if (string_size == 1)
                (*string_array)[*string_array_size - 1] = (char *) malloc (sizeof (char));
            else
                (*string_array)[*string_array_size - 1] = (char *) realloc ((*string_array)[*string_array_size - 1], sizeof(char) * string_size);

            assert (*string_array != NULL);

            if (string[i] == separator || string[i] == '\0')
            {
                printf("***found separator at %d\n", i);
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

    *string_array = (char **) realloc (*string_array, sizeof (char*) * size);
    assert (*string_array != NULL || *array_size);

    *array_size = size;
}

//===============================================================================================================

void delete_larger (char *** string_array, unsigned int * array_size, unsigned int N)
{
    unsigned int size = *array_size;  // local copy of array size
    int i = 0;                        // index

    while (i < size)
    {
        if (my_strlen ((*string_array)[i]) > N)
        {
            free ((*string_array)[i]);
            ROL (*string_array, i, size - 1);
            size--;
        }
        else
            i++;
    }

    *string_array = (char **) realloc (*string_array, sizeof (char*) * size);
    assert (*string_array != NULL || *array_size);

    *array_size = size;
}


//===============================================================================================================

int count_digits (int number)
{
    int count = 0;

    while (number)
    {
        number /= 10;
        count++;
    }

    return count;
}

//===============================================================================================================

char get_char_safe (const char *error_message, int (*filter) (int input))
{
    int input = getchar();

    if (input == EOF)
    {
        set_tmcolor (TM_COLOR_RED);
        printf ("Reached end of file, exiting...\n");
        reset_tmcolor();

        return 0;
    }

    while (flush_buffer() || !filter (input))
    {
        set_tmcolor (TM_COLOR_YELLOW);
        printf (error_message);
        reset_tmcolor();

        printf ("> ");
        input = getchar();
    }

    return (char) input;
}

//===============================================================================================================

int default_filter (int input)
{
    return 1;
}

//===============================================================================================================

int command_filter (int input)
{
    int command = input - '0';

    return command > 0 && command < 4;
}

int N_filter (int input)
{
    return input >= 0;
}

int get_int_safe (int *dest, const char *error_message, int (*filter) (int))
{
    int scanf_output = 0;

    scanf_output = scanf ("%d", dest);

    if (scanf_output == EOF)
    {
        set_tmcolor (TM_COLOR_RED);
        printf ("Reached end of file, exiting...\n");
        reset_tmcolor();

        return 0;
    }

    while (scanf_output != 1 || flush_buffer() || !filter (*dest))
    {
        set_tmcolor (TM_COLOR_YELLOW);
        printf (error_message);
        reset_tmcolor();

        printf ("> ");
        scanf_output = scanf ("%d", dest);
    }

    return 1;
}
