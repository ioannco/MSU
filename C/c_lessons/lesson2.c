#include <stdio.h>

int main1 ()
{
    int a, b;
    a = 3;
    b = 4;

    printf ("%d + %d = %d\n", a, b, a + b);

    return 0;
}

// %d %i    - целочисленный тип
// %o       - 8-ричная система
// %x       - 16-ричная система
// %u       - беззнаковое целое
// %ld      - long
// %lld     - long long
// %lu      - unsigned long
// %llu     - unsigntd long long
// %f       - float
// %ld      - double
// %Ld      - long double

int main2 ()
{
    char c = 'A';
    printf ("%c = %d\n", c, c);

    return 0;
}

int main3 ()
{
    float x = 3441.144f, y = 2.7f;

    printf ("%5.2f + %.1f = %.2f\n", x, y, x + y);

    return 0;
}

// %e   - научная
// %s   - string

int main ()
{
    int m = 0;
    float x = 0.f, y = 0.f;

    // сохраняем результат ввода
    m = scanf ("%f %f", &x, &y);
    
    // проверяем успешность ввода
    if (m != 2) 
    {
        // выводим сообщение об ошибке
        printf ("Scanf input error\n");
        return -1;
    }

    return 0;
}

// -3 * 4L -6/5 = -13L


