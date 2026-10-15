CC=gcc
CFLAGS=-I.

.PHONY: all clean

ifeq ($(shell uname -s),Darwin)
SUM_LIB=libsum.dylib
SUM_FLAGS=-dynamiclib -fPIC
SUM_RPATH=-Wl,-rpath,@executable_path
else
SUM_LIB=libsum.so
SUM_FLAGS=-fPIC -shared
SUM_RPATH=-Wl,-rpath,'$$ORIGIN'
endif

all: parallel_min_max process_memory parallel_sum

$(SUM_LIB): sum.c sum.h
	$(CC) $(CFLAGS) $(SUM_FLAGS) sum.c -o $(SUM_LIB)

parallel_sum: parallel_sum.c sum.h utils.o utils.h $(SUM_LIB)
	$(CC) $(CFLAGS) -pthread parallel_sum.c utils.o -L. -lsum $(SUM_RPATH) -o $@

process_memory: process_memory.c
	$(CC) -o process_memory process_memory.c

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h parallel_min_max.c
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.c utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)


clean :
	rm -f utils.o find_min_max.o psum libsum.so libsum.dylib
