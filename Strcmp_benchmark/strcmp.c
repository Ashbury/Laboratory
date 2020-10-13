#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 4096
#define NB_OF_TESTS 1000000

// ~2.095s total on my machine for main test (stdin to /dev/null) with clang -Ofast
int	rep_strcmp(char *s1, char *s2);

// ~1.385s total on my machine for main test (stdin to /dev/null) with clang -Ofast
int	tail_recursive_strcmp(char *s1, char *s2)
{
	return (*s1 && *s1 == *s2) ? tail_recursive_strcmp(s1 + 1, s2 + 1) : *s1 - *s2;
}

// ~2.150s total on my machine for main test (stdin to /dev/null) with clang -Ofast
int	classic_strcmp_no_var(char *s1, char *s2)
{
	while(*s1 && *s1++ == *s2++)
		;
	return *s1 - *s2;
}

// ~1.325s total on my machine for main test (stdin to /dev/null) with clang -Ofast
int	classic_strcmp_with_var(char *s1, char *s2)
{
	size_t i = 0;

	while(s1[i] && s1[i] == s2[i])
		i++;
	return s1[i] - s2[i];

}

// ~0.335s total on my machine for main test (stdin to /dev/null) with clang -Ofast and uint64_t
// ~0.280s total on my machine for main test (stdin to /dev/null) with clang -Ofast and uint128_t
int	grouped_strcmp(char *s1, char *s2)
{
	size_t len = strlen(s1);
	size_t i = 0;

	while(len > sizeof(__uint128_t))
	{
		if (*(__uint128_t*)(s1 + i) != *(__uint128_t*)(s2 + i))
			break ;
		i += sizeof(__uint128_t);
		len -= sizeof(__uint128_t);
	}
	while(s1[i] && s1[i] == s2[i])
		i++;
	return s1[i] - s2[i];
}

// system strcmp: ~0.135s total on my machine for main test (stdin to /dev/null) with clang -Ofast
int	main(void)
{
	char	s1[BUFF_SIZE];
	char	s2[BUFF_SIZE];

	memset(s1, 'A', BUFF_SIZE - 1);
	memset(s2, 'A', BUFF_SIZE - 1);
	s1[BUFF_SIZE - 1] = '\0';
	s2[BUFF_SIZE - 1] = '\0';

	for(size_t i = 0; i < NB_OF_TESTS; i++)
	{
		// Redirect stdin to /dev/null, we just want the compiler not to skip the call
		printf("%d\n", rep_strcmp(s1, s2));
	}
	return 0;
}
