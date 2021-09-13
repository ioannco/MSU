#include <stdio.h>
#include <stdlib.h>

enum commands 
{
    print_date_cmd = '0',
    print_day_cmd,
    print_month_cmd,
    print_year_cmd,
    set_day_cmd,
    set_month_cmd,
    set_year_cmd,
    print_binary_cmd
};

const unsigned short day_mask_c = 0x001F, month_mask_c = 0x01E0, year_mask_c = 0xFE00;

unsigned short date_to_compact (                     unsigned int   day, unsigned int   month, unsigned int   year);
void           compact_to_date (unsigned short date, unsigned int * day, unsigned int * month, unsigned int * year);

void           print_binary    (unsigned short  date);

unsigned short set_day         (unsigned short  date);
unsigned short set_month       (unsigned short  date);
unsigned short set_year        (unsigned short  date);

void           print_day       (unsigned short  date);
void           print_month     (unsigned short  date);
void           print_year      (unsigned short  date);

int main ()
{
    unsigned int day, month, year;
    unsigned short compact;
    
    while (1)
    {
        printf ("Enter date (DD MM YY): ");
        if (scanf (" %u %u %u", &day, &month, &year) != 3 || day > 31 || month > 12 || year > 99)
        {
            printf ("Sorry, incorrect date. Try again.\n");
            continue;
        }
        else
            compact = date_to_compact (day, month, year);
            break;
    }

    printf ("List of commands:\n\n");
    printf ("(0)     Print date\n");
    printf ("(1)     Print day\n");
    printf ("(2)     Print month\n");
    printf ("(3)     Print year\n");
    printf ("(4)     Set day\n");
    printf ("(5)     Set month");
    printf ("(6)     Set year\n");
    printf ("(7)     Print binary\n");
    printf ("(other) Exit.\n"); 

    while (1)
    {
        char input = ' ';
        printf ("\nEnter command: ");

        scanf (" %c", &input);

        switch (input)
        {
        case print_date_cmd:
            compact_to_date (compact, &day, &month, &year);
            printf ("Current date: %u//%u//%u\n", day, month, year);
            break;

        case print_day_cmd:
            break;
            
        case print_month_cmd:
            break;

        case print_year_cmd:
            break;

        case set_day_cmd:
            compact = set_day (compact);
            break;

        case set_month_cmd:
            compact = set_month (compact);
            break;

        case set_year_cmd:
            compact = set_year (compact);
            break;

        case print_binary_cmd:
            printf ("Stored date in binary: \n");
            print_binary (compact);
            printf ("\n");
            break;
            
        default:
            printf ("Bye!\n");
            exit (0);
        }

    }

}

unsigned short date_to_compact (unsigned int day, unsigned int month, unsigned int year)
{
    unsigned short compact = 0;

    compact = year;
    compact = compact << 4;
    compact += month;
    compact = compact << 5;
    compact += day;
    
    return compact;
}

void compact_to_date (unsigned short date, unsigned int * day, unsigned int * month, unsigned int * year)
{
    *day   =  date & day_mask_c;
    *month = (date & month_mask_c) >> 5;
    *year  = (date & year_mask_c) >> 9; 
}

void print_binary (unsigned short date)
{
    int i = 16;

    for (i = 15; i >= 0; i--)
    {
        int num = (date >> i) & 1;
        printf ("%d", num);
    }
}

unsigned short set_day (unsigned short date)
{
    unsigned short day = 0;
    
    fflush(stdin);

    printf ("Enter day: ");
    if (scanf ("%u", &day) != 1 || day > 31)
    {
        printf ("Incorrect day format\n");
        exit (0);
    }
    
    return (date & ~day_mask_c) | day;
}

unsigned short set_month (unsigned short date)
{
    unsigned short month = 0;

    printf ("Enter month: ");
    if (scanf (" %u", &month) != 1 || month > 12)
    {
        printf ("Incorrect month format\n");
        exit (0);
    }

    return (date & ~month_mask_c) | (month << 5); 
} 

unsigned short set_year (unsigned short date)
{
    unsigned short year = 0;

    printf ("Enter year: ");
    if (scanf ("%u", &year) != 1 || year > 99)
    {
        printf ("Incorrect year format\n");
        exit (0);
    }

    return (date & ~year_mask_c) | (year << 9); 
} 
