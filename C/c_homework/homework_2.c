//===============================================================================================================
// Includes
//===============================================================================================================

// STD
#include <stdio.h>
#include <stdlib.h>

// Terminal colors library include
#include "colors.h"

//===============================================================================================================
// Defines and constants
//===============================================================================================================

// EOF check macro
// output - scanf output code
#define CHECK_EOF(output)  if (output == EOF) { $frd printf ("\nSorry, reached end of input stream, aborting...\n\a"); $def exit(0); }

//===============================================================================================================

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

//===============================================================================================================
// Function declarations
//===============================================================================================================

// Pack date to more compact container or vice versa
unsigned short date_to_compact (                     unsigned int   day, unsigned int   month, unsigned int   year);
void           compact_to_date (unsigned short date, unsigned int * day, unsigned int * month, unsigned int * year);

// Print short value in binary representation
void           print_binary    (unsigned short date);

// Set day month, or year in packed date
unsigned short set_day         (unsigned short date);
unsigned short set_month       (unsigned short date);
unsigned short set_year        (unsigned short date);

// Get day month, or year from packed date and print it
void           print_day       (unsigned short date);
void           print_month     (unsigned short date);
void           print_year      (unsigned short date);

// Flush the console buffer
void           flush_input     ();

// Function that checks the date
int            check_date              (unsigned int day, unsigned int month, unsigned int year);
int            check_compact_date      (unsigned short date);

//===============================================================================================================
// Main
//===============================================================================================================

int main ()
{
    unsigned int day, month, year; // Date
    unsigned short compact;        // Compact container
    int running = 1;               // Exit condition
    int scanf_output = 0;          // Scanf output code

    // Asking to enter coefficients until they are correct
    while (1)
    {
        set_tmcolor (TM_COLOR_MAGENTA);
        printf ("Enter date (DD MM YY): ");

        set_tmcolor (TM_COLOR_WHITE);
        scanf_output = scanf ("%u %u %u", &day, &month, &year);
        CHECK_EOF(scanf_output);

        if (scanf_output != 3 || !check_date(day, month, year))
        {
            set_tmcolor (TM_COLOR_YELLOW);
            printf ("Sorry, incorrect date. Try again.\n\n");
            
            flush_input ();  // Clear input and try again
            continue;
        }

        compact = date_to_compact (day, month, year); // If input is correct, pack date to compact container
        break;
    }

    // Print list of commands
    set_tmcolor (TM_COLOR_BLUE);
    printf ("\nList of commands:\n\n");
    
    set_tmcolor (TM_COLOR_WHITE);
    printf ("(0)     Print date\n");
    printf ("(1)     Print day\n");
    printf ("(2)     Print month\n");
    printf ("(3)     Print year\n");
    printf ("(4)     Set day\n");
    printf ("(5)     Set month\n");
    printf ("(6)     Set year\n");
    printf ("(7)     Print binary\n");
    printf ("(other) Exit.\n"); 
    
    set_tmcolor (TM_COLOR_CYAN);
    // Main cycle
    while (running)
    {
        char input = ' ';                           // User input character

        set_tmcolor (TM_COLOR_MAGENTA);
        printf ("\nEnter command: ");        // Command input

        set_tmcolor (TM_COLOR_WHITE);
        scanf_output = scanf (" %c", &input);

        CHECK_EOF (scanf_output);

        // Command parse switch
        switch (input)
        {
        case PRINT_DATE_CMD:  // Print date
            compact_to_date (compact, &day, &month, &year);
            set_tmcolor (TM_COLOR_CYAN);
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
            $fgr
            printf ("Success!\n");
            break;

        case SET_MONTH_CMD:   // Set month
            compact = set_month (compact);
            $fgr
            printf ("Success!\n");

            break;

        case SET_YEAR_CMD:    // Set year
            compact = set_year (compact);
            $fgr
            printf ("Success!\n");
            break;

        case PRINT_BINARY_CMD: // Print compact container in binary representation
            set_tmcolor (TM_COLOR_CYAN);
            printf ("Stored date in binary: \n");
            print_binary (compact);
            printf ("\n");
            break;
            
        default:            // Exit if other command
        
            set_tmcolor (TM_COLOR_YELLOW);
            printf ("Bye!\n");
            running = 0;
            break;
        }
    }
    
    $def
    return 0;
}

//===============================================================================================================
// Function definitions
//===============================================================================================================

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

//===============================================================================================================

void compact_to_date (unsigned short date, unsigned int * day, unsigned int * month, unsigned int * year)
{
    // Using mask and shift to get value
    *day   =  date & day_mask_c;
    *month = (date & month_mask_c) >> 5;
    *year  = (date & year_mask_c) >> 9; 
}

//===============================================================================================================

void print_binary (unsigned short date)
{
    // Starting from the end
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

//===============================================================================================================

unsigned short set_day (unsigned short date)
{
    // Day value
    unsigned int day = 0;

    set_tmcolor (TM_COLOR_MAGENTA);
    printf ("Enter day: ");

    set_tmcolor (TM_COLOR_WHITE);
    while (1)
    {
        unsigned int month, year;
        compact_to_date (date, &day, &month, &year);
        int scanf_output = scanf ("%u", &day);
        CHECK_EOF (scanf_output);

        if (scanf_output == 1 && check_date (day, month, year))
            break;

        set_tmcolor (TM_COLOR_YELLOW);
        printf ("Incorrect day format, try again: ");

        flush_input(); // Try again
        set_tmcolor (TM_COLOR_WHITE);
    }

    return (date & ~day_mask_c) | (unsigned short) day;
}

//===============================================================================================================

unsigned short set_month (unsigned short date)
{
    // Month value
    unsigned int month = 0;

    set_tmcolor (TM_COLOR_MAGENTA);
    printf ("Enter month: ");

    set_tmcolor (TM_COLOR_WHITE);
    while (1)
    {
        unsigned int day, year;
        compact_to_date (date, &day, &month, &year);

        int scanf_output = scanf ("%u", &month);
        CHECK_EOF (scanf_output);

        if (scanf_output == 1 && check_date (day, month, year))
            break;

        set_tmcolor (TM_COLOR_YELLOW);
        printf ("Incorrect month format, try again: ");
        flush_input(); // Try again
        set_tmcolor (TM_COLOR_WHITE);
    }

    return (date & ~month_mask_c) | (unsigned short) (month << 5);
} 

//===============================================================================================================

unsigned short set_year (unsigned short date)
{
    // Year value
    unsigned int year = 0;

    set_tmcolor (TM_COLOR_MAGENTA);
    printf ("Enter year: ");

    set_tmcolor (TM_COLOR_WHITE);
    while (1)
    {
        unsigned int day, month;
        compact_to_date (date, &day, &month, &year);

        int scanf_output = scanf ("%u", &year);
        CHECK_EOF (scanf_output);

        if (scanf_output == 1 && check_date (day, month, year))
            break;

        set_tmcolor (TM_COLOR_YELLOW);
        printf ("Incorrect year format, try again: ");
        flush_input (); // Try again
        set_tmcolor (TM_COLOR_WHITE);
    }

    return (date & ~year_mask_c) | (unsigned short) (year << 9);
}

//===============================================================================================================

void print_day (unsigned short date)
{
    set_tmcolor (TM_COLOR_CYAN);
    printf ("Day: %u\n", date & day_mask_c);
}

//===============================================================================================================

void print_month (unsigned short date)
{
    set_tmcolor (TM_COLOR_CYAN);
    printf ("Month: %u\n", (date & month_mask_c) >> 5);
}

//===============================================================================================================

void print_year (unsigned short date)
{
    int year = (date & year_mask_c) >> 9;

    set_tmcolor (TM_COLOR_CYAN);
    printf ("Year: %u\n", year > 49 ? 1900 + year : 2000 + year);
}

//===============================================================================================================

void flush_input ()
{
    int c = 0;

    while ((c = getchar()) != '\n' && c != EOF);
}

//===============================================================================================================

int check_date (unsigned int day, unsigned int month, unsigned int year)
{
    if (!month || !day || year > 99)
        return 0;

    if (month == 2)
    {
        if (year % 4)
            return day < 29;

        return day < 30;
    }

    if (month < 8)
    {
        if (month % 2 == 0)
            return day < 31;

        return day < 32;
    }

    if (month < 13)
    {
        if (month % 2 == 0)
            return day < 32;

        return day < 31;
    }

    return 0;
}

//===============================================================================================================

int check_date_compact (unsigned short date)
{
    unsigned int day, month, year;

    compact_to_date (date, &day, &month, &year);
    return check_date (day, month, year);
};