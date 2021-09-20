#include <stdio.h>
#include <stdlib.h>

// Colors library include
#include "colors.h"

// ===============================================================================

// EOF check macro
// output - scanf output code
#ifndef CHECK_EOF

#define CHECK_EOF(output)  if (output == EOF) { $frd printf ("\nSorry, reached end of input stream, aborting...\n\a"); $def exit(0); }

#else
#error  CHECK_EOF define collision
#endif

// ===============================================================================

// Lists of commands
enum commands 
{
    PRINT_DATE_CMD = '0',
    PRINT_DAY_CMD,
    PRINT_MONTH_CMD,
    PRINT_YEAR_CMD,
    SET_DAY_CMD,
    SET_MONTH_CMD,
    SET_YEAR_CMD,
    PRINT_BINARY_CMD
};

// Compact data masks
const unsigned short day_mask_c = 0x001F, month_mask_c = 0x01E0, year_mask_c = 0xFE00;

// ===============================================================================

// Pack date to more compact container
unsigned short date_to_compact (                     unsigned int   day, unsigned int   month, unsigned int   year);
// Unpack date
void           compact_to_date (unsigned short date, unsigned int * day, unsigned int * month, unsigned int * year);

// Print short value in binary representation
void           print_binary    (unsigned short date);

// Set day in packed date
unsigned short set_day         (unsigned short date);
// Set month in packed date
unsigned short set_month       (unsigned short date);
// Set year in packed date
unsigned short set_year        (unsigned short date);

// Get day from packed date and print it
void           print_day       (unsigned short date);
// Get month from packed date and print it
void           print_month     (unsigned short date);
// Get year form packed date and print it
void           print_year      (unsigned short date);

// Flush the console buffer
void           flush_input     ();

// ===============================================================================

int main ()
{
    unsigned int day, month, year; // Date
    unsigned short compact;        // Compact container
    int running = 1;               // Exit condition
    int scanf_output = 0;          // Scanf output code

    // Asking to enter coefficients until they are correct
    while (1)
    { 
$fmg    printf ("Enter date (DD MM YY): "); 
        if ((scanf_output = scanf ("%u %u %u", &day, &month, &year)) != 3 || day > 31 || month > 12 || year > 99)
        {
            
            CHECK_EOF(scanf_output);
$fyl        printf ("Sorry, incorrect date. Try again.\n\n");
            flush_input ();  // Clear input and try again
            continue;
        }
        else
            compact = date_to_compact (day, month, year); // If input is correct, pack date to compact container
            break;
    }

    // Print list of commands
$fbl
    printf ("\nList of commands:\n\n");
$def
    printf ("(0)     Print date\n");
    printf ("(1)     Print day\n");
    printf ("(2)     Print month\n");
    printf ("(3)     Print year\n");
    printf ("(4)     Set day\n");
    printf ("(5)     Set month\n");
    printf ("(6)     Set year\n");
    printf ("(7)     Print binary\n");
    printf ("(other) Exit.\n"); 
$fmg
    // Main cycle
    while (running)
    {
        char input = ' ';                           // User input character

        printf ("\nEnter command: ");        // Command input
        scanf_output = scanf (" %c", &input);

        CHECK_EOF (scanf_output);

        // Command parse switch
        switch (input)
        {
        case PRINT_DATE_CMD:  // Print date
            compact_to_date (compact, &day, &month, &year);
            printf ("Current date: %u/%u/%u\n", day, month, year);
            break;

        case PRINT_DAY_CMD:   // Print day only
            print_day (compact);
            break;
            
        case PRINT_MONTH_CMD: // Print month only
            print_month (compact);
            break;

        case PRINT_YEAR_CMD:  // Print year only
            print_year (compact);
            break;

        case SET_DAY_CMD:     // Set day
            compact = set_day (compact);
            printf ("Success!\n");
            break;

        case SET_MONTH_CMD:   // Set month
            compact = set_month (compact);
            printf ("Success!\n");

            break;

        case SET_YEAR_CMD:    // Set year
            compact = set_year (compact);
            printf ("Success!\n");
            break;

        case PRINT_BINARY_CMD: // Print compact container in binary representation
            printf ("Stored date in binary: \n");
            print_binary (compact);
            printf ("\n");
            break;
            
        default:            // Exit if other command
$fyl        printf ("Bye!\n");
            running = 0;
            break;
        }

    }
$def
    return 0;
}

// ===============================================================================

unsigned short date_to_compact (unsigned int day, unsigned int month, unsigned int year)
{
    unsigned short compact = 0; // container to store date in

    // Set year and shift
    compact = year;
    compact = compact << 4;

    // Set month and shift
    compact += month;
    compact = compact << 5;

    // Set day
    compact += day;
    
    return compact;
}

// ===============================================================================

void compact_to_date (unsigned short date, unsigned int * day, unsigned int * month, unsigned int * year)
{
    // Using mask and shift to get value
    *day   =  date & day_mask_c;
    *month = (date & month_mask_c) >> 5;
    *year  = (date & year_mask_c) >> 9; 
}

// ===============================================================================

void print_binary (unsigned short date)
{
    // Starting from the eldest bit
    int i = 16;

    // Parsing bits
    for (i = 15; i >= 0; i--)
    {
        int num = (date >> i) & 1;
        printf ("%d", num);
        if (i == 8)
            printf (" ");
    }
}

// ===============================================================================

unsigned short set_day (unsigned short date)
{
    // Day value
    unsigned int day = 0;
    int scanf_output = 0;
    
    printf ("Enter day: ");
    while ((scanf_output = scanf ("%u", &day)) != 1 || day > 31)
    {
        CHECK_EOF (scanf_output);
        printf ("Incorrect day format, try again: ");
        flush_input(); // Try again
    }
    
    return (date & ~day_mask_c) | (unsigned short) day;
}

// ===============================================================================

unsigned short set_month (unsigned short date)
{
    // Month value
    unsigned int month = 0;
    int scanf_output = 0;

    printf ("Enter month: ");
    while (scanf_output = (scanf ("%u", &month)) != 1 || month > 12)
    {
        CHECK_EOF (scanf_output);
        printf ("Incorrect month format, try again: ");
        flush_input(); // Try again
    }

    return (date & ~month_mask_c) | (unsigned short) (month << 5);
} 

// ===============================================================================

unsigned short set_year (unsigned short date)
{
    // Year value
    unsigned int year = 0;
    int scanf_output = 0;

    printf ("Enter year: ");
    while (scanf_output = (scanf ("%u", &year)) != 1 || year > 99)
    {
        CHECK_EOF (scanf_output);

        printf ("Incorrect year format, try again: ");
        flush_input (); // Try again
    }

    return (date & ~year_mask_c) | (unsigned short) (year << 9);
}

// ===============================================================================

void print_day (unsigned short date)
{
    printf ("Day: %u\n", date & day_mask_c);
}

// ===============================================================================

void print_month (unsigned short date)
{
    printf ("Month: %u\n", (date & month_mask_c) >> 5);
}

// ===============================================================================

void print_year (unsigned short date)
{
    int year = (date & year_mask_c) >> 9;

    printf ("Year: %u\n", year > 49 ? 1900 + year : 2000 + year);
}

// ===============================================================================

void flush_input ()
{
    int c = 0;

    while ((c = getchar()) != '\n' && c != EOF);
}

// ===============================================================================
