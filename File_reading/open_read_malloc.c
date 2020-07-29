#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int	main(int ac, char **av)
{
	int		fd;
	struct stat	file_stats;
	char	*buffer;

	if (ac != 2)
	{
		dprintf(2, "Usage: %s <file>\n", av[0]);
		exit(1);
	}

	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		dprintf(2, "Error while opening file.\n");
		exit(2);
	}
	if (fstat(fd, &file_stats) < 0)
	{
		close(fd);
		dprintf(2, "Error with fstat on file.\n");
		exit(3);
	}

	buffer = malloc(file_stats.st_size);
	if (buffer == NULL)
	{
		dprintf(2, "Error: couldn't allocate memory\n");
		exit(4);
	}

	read(fd, buffer, file_stats.st_size);
	write(1, buffer, file_stats.st_size);
	free(buffer);
	close(fd);
	return 0;
}
