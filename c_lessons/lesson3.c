// ===================================================================================
// Циклы
// ===================================================================================

int main1 ()
{
    int i, j;

    for (/*Начальное значение*/ i = 0; /*Условие выхода*/ i < 10; /*Шаг*/ i++)
    {
    }

    // Доп. условия идут через запятую
    for (i = 0, j = 10; i < j; i++, j++)
    {
    }

    // Бескнечный цикл
    for (;;)
    {
    }
    // или
    while (1)
    {
    }

    // Досрочный выход из цикла:
    // break;
    //
    // Пропустить итерацию:
    // continue; 

    return 0;
}

// Определить, есть ли в числе цифра 3
int has_3 (unsigned num)
{
    while (num)
    {
        if (num % 10 == 3)
            break;
        num /= 10;
    }

    return num != 0;
}

// Посчитать сумму цифр без цифры 3
unsigned sum_without_3 (unsigned num)
{
    unsigned sum = 0u;

    while (num)
    {
        int digit = num % 10;
        num /= 10;

        if (digit == 3)
            continue;

        sum += digit;
    }

    return sum;
}

// ===================================================================================
// Функции
// ===================================================================================
#include <stdio.h>

float rub_to_euro (float t);

int main2 ()
{
    float rub = 0.f, euro = 0.f;

    scanf ("%f", &rub);

    euro = rub_to_euro (rub);

    printf ("%f\n", euro);
    return 0;
}

float rub_to_euro (float t)
{
    return t / 86.92;
}


// ===================================================================================
// Перечислимый тип
// ===================================================================================

enum color 
{
    red     = 0xff0000,
    blue    = 0x0000ff,
    green   = 0x00ff00,
    white   = 0xffffff,
    black   = 0x000000
};

int main ()
{
    enum color my_color = red;

    if (my_color == red)
        printf ("red");
    else if (my_color == blue)
        printf ("blue");


   switch (my_color)
   {
       case red:
           printf ("red\n");
           break;

       case green:
           printf ("green\n");
           break;

       case blue:
           printf ("blue\n");
           break;

       default:
           printf ("unknown color\n");
           break;
   } 
}


// ===================================================================================
// Задание констант
// ===================================================================================

// const

// 1)
const int n = 4;

// 2)
#define N 4
