#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	printf("Pid: %d\n", getpid());
	if (fork() == 0)
		execve("print_pid", NULL, NULL);
	return 0;
}
