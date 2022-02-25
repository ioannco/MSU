#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main ()
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return 1;
	}
	else if (!pid)
		printf ("SON PID = %d; PPID = %d\n", getpid(), getppid());
	else 
		printf ("FATHER PID = %d; PPID = %d\n", getpid(), getppid());
	return 0;
}
