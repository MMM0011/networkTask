CC=gcc

CFLAGS=-pthread

all: clean main

main: clientServer.o
	$(CC) $(CFLAGS) clientServer.o -o clientServer

clientServer.o:
	$(CC) $(CFLAGS) -c clientServer.c

clean: 
	rm -rf *.o clientServer
