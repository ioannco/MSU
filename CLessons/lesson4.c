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

void packing (unsigned short * pack, int day, int month, int year)
{
    day = day << 11;

    *pack = *pack | day;
    *pack = *pack | (month << 7);
}

int main ()
{
    int var = 0;

    unsigned short pack = 0;

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



