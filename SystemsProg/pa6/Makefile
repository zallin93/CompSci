CC=gcc
CFLAGS=-Wall -g

all: test
	

othermalloc.o:
	$(CC) $(CFLAGS) -c othermalloc.c

test: othermalloc.o test.o
	$(CC) $(CFLAGS) othermalloc.o test.o -o test

test.o:
	$(CC) $(CFLAGS) -c test.c

clean:
	rm -rf *.o test  a.out