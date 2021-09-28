#include <stdio.h>
#include <stdlib.h>

int main1 ()
{
    int k;
    k = ~(~0<<2);

    // k = 3
    //
    // 11..000
    // 11..100
    // 00..011
    
    return 0;
}

int main ()
{
    short pack = (short) rand();
    short day = 13;

    printf ("pack = 0x%X\n", pack);
    printf ("day = %d = 0x%X\n", day, day);

    pack = pack & 0x07FF | (day << 11);

    printf ("pack with day = 0x%X\n", pack);

    return 0; 
}

// Приоритет операций
//
// 1) [] . -> ()
// 2) &, *, ++, --, ~, !, -
// 3) *, /, %
// 4) +, -
// 5) >>, <<
// 6) <, >, <=, >=
// 7) ==, !=
// 8) &
// 9) ^
// 10) |
// 11) &&
// 12) ||
// 13) ? :
// 14) =, +=, ...
// 15) ,



