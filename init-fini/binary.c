#include <stdio.h>
#include <stdlib.h>

void do_atexit(void)
{
	printf ("binary:: do_atexit\n");
}

#define UNREFERENCED_PARAMETER(x) ((x)=(x))

int main (int argc, char *argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	printf ("binary:: starting main\n");
	atexit (do_atexit);
	printf ("binary:: finishing main\n");
	return 0;
}

