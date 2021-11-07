#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argv, char ** argc)
{
    pid_t pid = fork();
    char input = 0;
    int fd = -1;
    int key = pid ? 1 : 2;
    char buf[1];

    buf[0] = key + '0';

    printf ("pid = %d, key = %d\n", getpid(), key);
    
    if (argv < 2)
        return 0;


    fd = open (argc[1], O_RDWR);
    if (!fd)
        return 1;
    
    while (read (fd, &input, 1))
    {
        printf ("[%d] %c\n", getpid(), input);
        if (input - '0' == key)
        {
            lseek (fd, -1, SEEK_CUR);
            while (read (fd, &input, 1))
            {
                printf ("[%d] %c\n", getpid(), input);
                if (input == ' ')
                    break;
                lseek (fd, -1, SEEK_CUR);
                write (fd, buf, 1);
            }
        }
    }

    return 0;
}

