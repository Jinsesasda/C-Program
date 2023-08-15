CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -fstack-protector-all -Werror

longlines2-threaded.o: longlines2-threaded.c randomdelay.h
	$(CC) $(CFLAGS) -c longlines2-threaded.c

longlines2-threaded.x: longlines2-threaded.o randomdelay.o
	$(CC) longlines2-threaded.o randomdelay.o -o longlines2-threaded.x

clean: rm -f *.x longlines2-threaded.o
