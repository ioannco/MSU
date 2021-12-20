#include <unistd.h>
#include <memory.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main(int argc, char **argv)
{
    int key = ftok (argv[0], 'a');
    int shm_key = shmget (key, 256, 
}
