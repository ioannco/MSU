#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main (int argc, char ** argv)
{
    char * str[4] = {"sin", "cos", "exp", "fabs"};
    double (*m[4])(double) = {sin, cos, exp, fabs};
    double x;
    int i;

    for (i = 0; i < 4; i++)
    {
        if (!strcmp (str[i], argv[1]))
            break;
    }

    if (i == 4)
    {
        printf ("don't know this function");
        return 1;
    }

    x = atof (argv[2]);

    if (x == 0 && (argv[2][0] != '0' || !strcmp ("-0", argv[2])))
    {
        fprintf (stderr, "x is not number\n");
        return 2;
    }

    printf ("%s(%lf) = %lf\n", str[i], x, m[i](x));
    return 0;
}