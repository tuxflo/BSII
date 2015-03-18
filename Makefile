CC=gcc
CFLAGS=-Wall -std=c11
all: 
	$(CC) $(CFLAGS) aufg1.c -o aufg1.o

clean:
	rm *.o
	rm aufg*
