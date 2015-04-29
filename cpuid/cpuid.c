#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define UNREFERENCED_PARAMETER(x) ((x)=(x))

int main (int argc, char *argv[])
{
	char name[13] = { 0 };
	uint32_t eax, ebx, ecx, edx;
	uint32_t maxleaf;

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	uint32_t op = 0;
	asm ( "cpuid \n"
       : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
       : "a" (op));

	*((uint32_t*)(&name[0])) = ebx;
	*((uint32_t*)(&name[4])) = edx;
	*((uint32_t*)(&name[8])) = ecx;

	printf ("name = %s eax = %08x\n", name, eax);

	maxleaf = eax;
	
	op = 1;
	asm ("cpuid"
       : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
       : "a" (op));

	printf ("cpuid %u eax = %08x, ebx = %08x, ecx = %08x, edx = %08x\n", op, eax, ebx, ecx, edx);

	if (maxleaf >= 7)
	{
		op = 7;
		asm ("xorq %%rcx, %%rcx \n cpuid"
  	     : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    	   : "a" (op));
		printf ("cpuid %u / %u eax = %08x, ebx = %08x, ecx = %08x, edx = %08x\n", op, 0, eax, ebx, ecx, edx);
	}

	return 0;
}

