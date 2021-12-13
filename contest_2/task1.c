//
// Created by Ivan on 12/13/2021.
//

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
    int fd = open (argv[1], O_RDWR);
    unsigned char buffer[sizeof (unsigned int)];

    while (read (fd, &buffer, sizeof (unsigned int)) > 0)
    {
        size_t i;

        for (i = 0u; i < sizeof (unsigned int); i++)
        {
            buffer[i] = ((buffer[i] >> 4) & 0x0f) + ((buffer[i] << 4) & 0xf0);
        }

        lseek (fd, -sizeof(unsigned int), SEEK_CUR);

        write (fd, &buffer, sizeof (unsigned int));
    }
}