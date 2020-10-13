#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 32

int	main(void);

// I know globals are initialized to zero by default, but I prefer to be clear.
size_t	test_number = 0;
size_t	idx = 0;
uint8_t	*array = NULL;
struct sigaction sa;

void	segfault_caught(int sig, siginfo_t *info, void *ucontext)
{
	(void)sig;
	(void)info;
	(void)ucontext;
	test_number += 1;
	printf("Test crashed at index %zu\nArray addres: %p\n", idx, array);
	free(array);
	printf("Calling main\n");
	main();
}

void	test_read_forward(void)
{
	array = malloc(sizeof(uint8_t) * ARRAY_SIZE);
	if (array == NULL)
	{
		dprintf(2, "Fatal: couldn't allocate memory\n");
		exit(1);
	}
	uint8_t	value;

	idx = 0;
	printf("Testing true forward reading range for uint8_t array of size %d\n", ARRAY_SIZE);
	while(1)
	{
		value = array[idx];
		idx++;
	}
}

void	test_write_forward(void)
{
	array = malloc(sizeof(uint8_t) * ARRAY_SIZE);
	if (array == NULL)
	{
		dprintf(2, "Fatal: couldn't allocate memory\n");
		exit(1);
	}

	idx = 0;
	printf("Testing true forward writing range for uint8_t array of size %d\n", ARRAY_SIZE);
	while(1)
	{
		array[idx] = 42;
		idx++;
	}
}

void	test_read_backward(void)
{
	array = malloc(sizeof(uint8_t) * ARRAY_SIZE);
	if (array == NULL)
	{
		dprintf(2, "Fatal: couldn't allocate memory\n");
		exit(1);
	}
	uint8_t *tmp = array;
	uint8_t	value;

	idx = 0;
	printf("Testing true backward reading range for uint8_t array of size %d\n", ARRAY_SIZE);
	while(1)
	{
		value = *tmp;
		tmp--;
		idx++;
	}
}

void	test_write_backward(void)
{
	array = malloc(sizeof(uint8_t) * ARRAY_SIZE);
	if (array == NULL)
	{
		dprintf(2, "Fatal: couldn't allocate memory\n");
		exit(1);
	}
	uint8_t *tmp = array;

	idx = 0;
	printf("Testing true backward writing range for uint8_t array of size %d\n", ARRAY_SIZE);
	while(1)
	{
		*array = 42;
		array--;
		idx++;
	}
}

int	main(void)
{
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = segfault_caught;
	sa.sa_flags = SA_SIGINFO | SA_NODEFER;
	sigaction(SIGSEGV, &sa, NULL);

	switch (test_number)
	{
		case 0:
			test_read_forward();
			break;
		case 1:
			test_write_forward();
			break;
		case 2:
			test_read_backward();
			break;
		case 3:
			test_write_backward();
			break;
		default:
			exit(1);
	}
	return 0;
}