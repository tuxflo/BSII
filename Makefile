CC=gcc
CFLAGS=-Wall
all: 
	$(CC) $(CFLAGS) aufg1.c -o aufg1

clean:
	rm *.o
