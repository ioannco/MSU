//===============================================================================================================
// Includes
//===============================================================================================================

// STD
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// My libs
#include "colors.h"

//===============================================================================================================
// Function declarations
//===============================================================================================================

// Start of quicksort algorithm
void quick_sort (char * array, unsigned int size);

// Recursive part of quicksort algoritrm
void qsort_r    (char * array, unsigned int left, unsigned int right);

// Partition of the array
int  partition  (char * array, unsigned int left, unsigned int right);

// Swap function
void swapc (char * a, char * b);

int sort_test (unsigned int array_length);

int comp (const char * left, const char * right);

//===============================================================================================================
// Main
//===============================================================================================================

int main (int argc, char ** argv)
{
    unsigned int i = 0;

    srand (time(NULL));

    for (i = 0; i < 1000; i++)
    {
        unsigned int array_size = rand() % 1000;

        printf ("Test #%4u. array_size = %4u. ", i, array_size);

        if (sort_test (array_size))
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
        }
    }
}

//===============================================================================================================
// Function definitions
//===============================================================================================================

void quick_sort (char * arr, unsigned int size)
{
    if (!size)
        return;

    qsort_r (arr, 0, size - 1);
}

//===============================================================================================================

void qsort_r (char * array, unsigned int left, unsigned int right)
{
    unsigned int p = 0;

    if (left >= right)
        return;

    p = partition (array, left, right);
    qsort_r (array, left, p);
    qsort_r (array, p + 1, right);
}

//===============================================================================================================

int partition (char * array, unsigned int left, unsigned int right)
{
    // i - left index, j - right index, p - pivot
    unsigned int i = left, j = right;
    char p = array[(left + right) / 2];

    while (1)
    {
        while (array[i] < p)
            i++;

        while (array[j] > p)
            j--;

        if (i >= j)
            return j;

        swapc (array + i++, array + j--);
    }
}

//===============================================================================================================

void swapc (char * a, char * b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

//===============================================================================================================

int sort_test (unsigned int array_size)
{
    int i = 0;
    char * array1 = malloc (array_size);
    char * array2 = malloc (array_size);

    for (i = 0; i < array_size; i++)
    {
        array1[i] = array2[i] = (char) (rand () % 256);
    }

    quick_sort (array1, array_size);
    qsort (array2, array_size, sizeof (char), (int(*) (const void *, const void *)) comp);

    for (i = 0; i < array_size; i++)
    {
        if (array1[i] != array2[i])
        {
            printf ("\nSort test failed. Arrays dump:\n");
            printf ("array1: ");

            for (i = 0; i < array_size; i++)
                printf ("%3d ", array1[i]);

            printf ("\narray2: ");

            for (i = 0; i < array_size; i++)
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

int comp (const char * a, const char * b)
{
    return *a - *b;
}