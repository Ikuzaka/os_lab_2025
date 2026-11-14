CC = gcc

.PHONY: all clean

all: client server

libnet.so: net.c net.h
	$(CC) -fPIC -shared -o libnet.so net.c

client: client.c net.h libnet.so
	$(CC) -o client client.c -L. -Wl,-rpath,'$$ORIGIN' -lnet

server: server.c net.h libnet.so
	$(CC) -pthread -o server server.c -L. -Wl,-rpath,'$$ORIGIN' -lnet

clean:
	rm -f client server libnet.so
