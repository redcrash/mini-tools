
all: library.c binary.c
	gcc -Wall -Wextra -W -O binary.c -o binary
	gcc -Wall -Wextra -W -O -g -fPIC -DPIC -shared library.c -o library.so
	LD_PRELOAD=./library.so ./binary
	rm -fr binary library.so
