CC=gcc
CFLAGS=-Wall -D_XOPEN_SOURCE=700 -std=c11
all:
	$(CC) $(CFLAGS) ipc.c -o ipc.o

clean:
	rm *.o

reset:
	rm zahl.dat && echo "0" > zahl.dat
