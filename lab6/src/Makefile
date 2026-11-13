CC = gcc
CPPFLAGS ?=
CFLAGS ?= -O2 -Wall -Wextra
LDFLAGS ?=
LDLIBS ?=

.PHONY: all clean

all: client server

libnet.so: net.c net.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ net.c

client: client.c net.h libnet.so
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ client.c -L. -Wl,-rpath,'$$ORIGIN' -lnet $(LDLIBS)

server: server.c net.h libnet.so
	$(CC) $(CPPFLAGS) $(CFLAGS) -pthread $(LDFLAGS) -o $@ server.c -L. -Wl,-rpath,'$$ORIGIN' -lnet $(LDLIBS)

clean:
	rm -f client server libnet.so
