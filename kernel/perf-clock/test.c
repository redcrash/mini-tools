#include <stdio.h>
#include <time.h>

int main (int argc, char *argv[])
{
	struct timespec ts;

#define CLOCK_PERF         12

	if (clock_gettime(CLOCK_PERF, &ts) != 0)
		printf("Failed to get perf_clock timestamp. perf_clock module loaded?\n");

	printf ("ts.tv_sec = %ld ts.tv_nsec = %ld\n", ts.tv_sec, ts.tv_nsec);

	return 0;
}
