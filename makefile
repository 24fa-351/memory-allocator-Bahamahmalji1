CC = gcc
CFLAGS = -Wall

all: test

test: test_memory_manager.o memory_manager.o
	$(CC) $(CFLAGS) -o test test_memory_manager.o memory_manager.o

test_memory_manager.o: test_memory_manager.c memory_manager.h
	$(CC) $(CFLAGS) -c test_memory_manager.c

memory_manager.o: memory_manager.c memory_manager.h
	$(CC) $(CFLAGS) -c memory_manager.c

clean:
	rm -f *.o test
