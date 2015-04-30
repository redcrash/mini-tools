#include <stdio.h>
#include <stdlib.h>

void do_atexit_library(void)
{
	printf ("library:: do_atexit_library\n");
}

__attribute__((constructor))
void library_init (void)
{
	printf ("library:: library_init\n");
	atexit (do_atexit_library);
}

__attribute__((destructor))
void library_fini (void)
{
	printf ("library:: library_fini\n");
}

