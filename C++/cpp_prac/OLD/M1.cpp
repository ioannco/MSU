//
// Created by Ivan on 2/15/2022.
//

#include <cmath>
#include <iostream>

int count_symbols (int arg);
int count_symbols (int * arr, int n);
int count_symbols (int * ptr);
int count_symbols (char * str);
int count_symbols (double f, int n = 0);

int count_symbols (int arg)
{
    if (!arg)
        return 1;

    int i = 0;

    for (i; arg; i++)
        arg /= 10;

    return i;
}

int count_symbols (int * arr, int n)
{
    int sum = 0;

    for (int i = 0; i < n; i++)
        sum += count_symbols (arr[i]);

    return sum;
}

int count_symbols (int * ptr)
{
    return count_symbols (*ptr);
}

int count_symbols (char * str)
{
    int n = 0;
    for (n; *str; n++, str++);
    return n;
}

int count_symbols (double f, int n)
{
    if (n)
        return count_symbols (static_cast<int> (f)) + count_symbols (static_cast<int> ((f - floor(f)) * pow(10, n)));
    else
        return count_symbols (static_cast<int> (f));
}


