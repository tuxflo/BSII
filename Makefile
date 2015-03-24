CC=gcc
CFLAGS=-Wall -std=c11
all: 
	$(CC) $(CFLAGS) aufg1.c -o aufg1.o

clean:
	rm *.o

reset:
	rm zahl.dat && echo "0" > zahl.dat
