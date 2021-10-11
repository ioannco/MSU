#include <stdio.h>

/**
 * @brief converts hex character to int
 *
 * @param input character
 * @return number or 0 if character is not hex number
 */
int hex_to_int (char input);

// ======================================================================================

int main()
{
    int input, sum = 0; // input - input from console; sum - sum of all hex numbers;

    while ((input = getchar()) != EOF)
        sum += hex_to_int ((char) input);

    printf ("%d\n", sum);
}

// =====================================================================================

int hex_to_int (char input)
{
    if (input >= '0' && input <= '9')
        return input - '0';

    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;

    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;

    return 0;
}
