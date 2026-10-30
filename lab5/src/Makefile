CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -O0
LDLIBS = -pthread

.PHONY: all clean
all: mutex_without mutex_with factorial deadlock

mutex_without: mutex.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

mutex_with: mutex.c
	$(CC) $(CFLAGS) -DUSE_MUTEX $< -o $@ $(LDLIBS)

factorial: factorial.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

deadlock: deadlock.c
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

clean:
	rm -f mutex_without mutex_with factorial deadlock
