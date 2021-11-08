#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

struct config_node 
{
    char key, replacement;
};

struct config_node * read_config (char * filename, int * process_count);
int process_text (char * filename, struct config_node * config, int my_proc_id);
int fstrlen (int fd);
int skip_word (int fd);

int main (int argv, char ** argc)
{
    int process_count = 0;
    struct config_node * config = NULL;
    int i = 0;

    if (argv != 3)
    {
        printf ("error: filenames params required\n"); 
        return -1;
    }  

    config = read_config (argc[2], &process_count);
    
    if (!config)
       return 1; 
    
    for (i = process_count - 1; i > 0; i--)
    {
        if (fork())
            break;
    }

    printf ("[PID %d]: id = %d. My key is %c and replacement is %c. Starting processing..\n", getpid(), i, config[i].key, config[i].replacement);

    process_text (argc[1], config, i);

    printf ("[PID %d]: id = %d. Finished processing.\n", getpid(), i);
    free (config);

    if (!i)
    {
        for (i = 0; i < process_count - 1; i++)
        {
            pid_t child_pid = wait(NULL);

            if (child_pid > 0) 
                printf ("[PID %d] father: child %d successifully terminated\n", getpid(), child_pid);
            else
                printf ("[pid %d] father: wait returned an error.\n", getpid());
        }
    }

    return 0;
}

struct config_node * read_config (char * filename, int * process_count)
{
    int i = 0;
    int input = 0;
    struct config_node * config = NULL;

    FILE * config_file = fopen (filename, "r");
    
    if (!config_file)
    {
        printf ("error while opening config file: %s\n", strerror (errno));
        return NULL;
    }
    
    if (fscanf (config_file, "%d", process_count) != 1)
    {
        printf ("config error: error while reading process count\n");
        fclose (config_file);
        return NULL;
    }
  
    if (*process_count < 1)
    {
        printf("config error: process count is less than 1\n");
        fclose (config_file);
        return NULL;
    }

    while ((input = fgetc (config_file)) == ' ')
    {
    }

    if (input != '\n')
    {
        printf ("config error: excess symbols in first string\n");
        fclose (config_file);
        return NULL;
    }

    config = (struct config_node *) malloc (sizeof (struct config_node) * (*process_count));
    assert (config != NULL);
   
    for (i = 0; i < *process_count; i++)
    {
        int j = 0;

        while ((input = fgetc(config_file)) == ' ')
        {
        }
        
        if (input == '*')
        {
            printf ("config error: '*' is not allowed to be a key\n");
            fclose (config_file);
            free (config);
            return NULL;
        }

        if (input == '\n')
        {
            printf ("config error: key is missing in string #%d\n", i);
            fclose (config_file);
            free (config);
            return NULL;
        } 

        config[i].key = (char) input;
        
        while ((input = fgetc(config_file)) == ' ')
        {
        }

        if (input == '\n')
        {
            printf ("config error: missing replacement to '%c' key in string #%d\n", config[i].key, i + 2);
            fclose (config_file);
            free (config);
            return NULL;  
        }

        config[i].replacement = (char) input;

        for (j = 0; j < i; j++)
            if (config[i].key == config[j].key)
            {
                printf ("config error: keys conflict\n");
                fclose (config_file);
                free (config);
                return NULL;
            }

        while ((input = fgetc(config_file)) == ' ')
        {
        }

        if (input != '\n')
        {
            printf ("config error: excess symbols in string #%d\n", i + 2);
            free (config);
            fclose (config_file);
            return NULL;    
        }
    }

    fclose (config_file);
    return config;
}

int process_text (char * filename, struct config_node * config, int my_proc_id)
{
    int fd = 0;
    char buffer = 0;
    int word_length = 0;
    char replacement_buffer[16] = "";

    assert (config != NULL);
    assert (filename != NULL);

    fd = open (filename, O_RDWR);

    if (fd < 0)
    {  
        printf ("error while opening text file: %s\n", strerror (errno));
        close(fd);
        return 1;
    }

    while (read (fd, &buffer, 1) > 0)
    {
        off_t word_start = 0;

        if (buffer != config[my_proc_id].key)
        {
            if (buffer != ' ' && buffer != ',' && buffer != '.')
                if (skip_word(fd))
                    break;
            continue;
        }

        if (buffer == '*')
        {
            if (skip_word (fd))
                break;
            continue;
        }

        assert (lseek (fd, -1, SEEK_CUR) >= 0);

        word_length = fstrlen (fd);
        if (word_length < 10)
        {
            if (skip_word (fd))
                break;
            continue;
        }

        word_start = lseek (fd, 0, SEEK_CUR);
        assert (word_start >= 0);

        assert (write (fd, "*", 1) != -1);
        assert (write (fd, &(config[my_proc_id].replacement), 1) != -1);
        
        if (my_proc_id)
        {
            sprintf (replacement_buffer, "SON%d", my_proc_id);

        }
        else
        {
            sprintf (replacement_buffer, "FATHER"); 
        }

        
        assert (lseek (fd, word_start + word_length - strlen (replacement_buffer), SEEK_SET) != -1);
        assert (write (fd, replacement_buffer, strlen (replacement_buffer)) != -1);
    }

    if (close (fd) == -1)
    {
        printf ("failed to close file: %s\n", strerror (errno));
    }
    return 0;
}

int fstrlen (int fd)
{
    char buffer = 0;
    off_t start_position = lseek (fd, 0, SEEK_CUR);
    int length = 0;

    assert (fd >= 0);
    assert (start_position >= 0);

    while (read (fd, &buffer, 1) > 0)
    {
        if (buffer == ' ' || buffer == ',' || buffer == '.')
            break;

        length++;
    }

    assert (lseek (fd, start_position, SEEK_SET) != -1);
    return length;
}

int skip_word (int fd)
{
    char buffer = 0;
        
    assert (fd >= 0);

    while (read (fd, &buffer, 1) > 0)
    {
        if (buffer == ' ' || buffer == '.' || buffer == ',')
        {
            assert (lseek (fd, -1, SEEK_CUR) != -1);
            return 0;
        }
    }

    return 1;
}
