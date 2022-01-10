CFLAGS=-ggdb3 -Wall -std=c99 -D_POSIX_SOURCE -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700
all: mm_test

mm_test: mm_test.o mm_alloc.o
	gcc $(CFLAGS) -o mm_test mm_alloc.o mm_test.o

mm_alloc.o: mm_alloc.c mm_alloc.h
	gcc $(CFLAGS) -c mm_alloc.c 

mm_test.o: mm_test.c mm_alloc.h
	gcc $(CFLAGS) -c mm_test.c

clean:
	rm *.o mm_test
