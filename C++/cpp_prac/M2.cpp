//
// Created by Ivan on 2/15/2022.
//

#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

char * left (char * str, int N);
unsigned long long left (unsigned long long num, int N);

int main ()
{
    char input [128];
    int N;

    std::cin >> input;

    if (strcmp (input, "string") == 0)
    {
        std::cin.ignore ();
        std::cin.get (input, 128);
        std::cin >> N;
        std::cout << left (input, N) << std::endl;
    }
    else if (strcmp (input, "number") == 0)
    {
        unsigned long long num = 0;
        std::cin >> num;
        std::cin >> N;
        std::cout << left (num, N) << std::endl;
    }

    return 0;
}

char * left (char * str, int N)
{
    if (!str || N < 0)
        return nullptr;

    if (strlen (str) < N)
        return str;

    str[N] = '\0';
    return str;
}

unsigned long long left (unsigned long long num, int N)
{
    if (N <= 0)
        return 0;

    int length = 0;
    for (unsigned long long num_cpy = num; num_cpy; length++, num_cpy /= 10);

    if (N >= length)
        return num;

    num /= static_cast<unsigned long long> (pow (10, static_cast<int> (length - N)));

    return num;
}