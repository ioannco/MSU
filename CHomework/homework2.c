#include <stdio.h>
#define BUFFER_SIZE 32

short str_to_compact (             char date_str[BUFFER_SIZE]);
void  compact_to_str (short  date, char date_str[BUFFER_SIZE]);

void  print_binary   (short  date);

short set_day        (short  date);
short set_month      (short  date);
short set_year       (short  date);

void  print_day      (short  date);
void  print_month    (short  date);
void  print_year     (short  date);

int main ()
{

}

short str_to_compact (char date_str[BUFFER_SIZE])
{
    short date = 0;
    unsigned day = 0, month = 0, year = 0;

    if (sscanf (date_str, "%d %d %d", &day, &month, &year) != 3)
    {
        printf ("
    }
    
    
}
