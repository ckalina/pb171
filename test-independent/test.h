#ifndef TEST_H
# define TEST_H
# include <stdio.h>
# include <stdlib.h>
static inline void TEST_ASSERT(int line, int result)
{
	if (result)
		return;

	fprintf(stderr, "Test failed on line %d.\n", line);

	exit(1);
}
#endif
