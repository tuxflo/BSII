CC=gcc
CFLAGS=-Wall -std=c11
all:
	$(CC) $(CFLAGS) aufg2.c -o aufg2.o

clean:
	rm *.o

reset:
	rm zahl.dat && echo "0" > zahl.dat
