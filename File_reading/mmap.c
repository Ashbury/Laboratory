#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int	main(int ac, char **av)
{
	int			fd;
	struct stat	file_stats;
	char *file;

	if (ac != 2)
	{
		dprintf(2, "Usage: %s <file>\n", av[0]);
		exit(1);
	}

	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		dprintf(2, "Error: could not open %s\n", av[1]);
		exit(2);
	}

	if (fstat(fd, &file_stats) < 0)
	{
		close(fd);
		dprintf(2, "Error with fstat on %s.\n", av[1]);
		exit(3);
	}

	if ((file = mmap(0, file_stats.st_size, PROT_READ,
			MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		close(fd);
		dprintf(2, "Error while mapping the file\n");
		exit(4);
	}
	close(fd);
	write(1, file, file_stats.st_size);
	munmap(file, file_stats.st_size);
	return 0;
}