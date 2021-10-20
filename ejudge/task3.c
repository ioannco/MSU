#include <stdio.h>

int main ()
{
    int a = 0, b = 0, n = 0;
    long long i = 0, j = 0;
    int table_size = -1;

    scanf ("%d %d %d", &a, &b, &n);

    table_size = b - a;

    printf ("%*s", n, " ");
    for (i = 0; i < table_size; i++)
    {
        printf (" %*lld", n, i + a);
    }
    printf ("\n");

    for (i = 0; i < table_size; i++)
    {
        printf ("%*lld", n, i + a);

        for (j = 0; j < table_size; j++)
            printf (" %*lld", n, (i + (long long) a) * (j + (long long) a));

        printf ("\n");
    }
}