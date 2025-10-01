CC = gcc
CFLAGS = -Wall -g

all: threaded_sum

threaded_sum: threaded_sum.o
	$(CC) $(CFLAGS) threaded_sum.o -o threaded_sum

threaded_sum.o: threaded_sum.c
	$(CC) $(CFLAGS) -c threaded_sum.c

clean:
	rm -f threaded_sum threaded_sum.o