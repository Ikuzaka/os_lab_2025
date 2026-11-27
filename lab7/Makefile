all: tcpserver tcpclient udpserver udpclient

tcpserver: src/tcpserver.c
	gcc -o tcpserver src/tcpserver.c

tcpclient: src/tcpclient.c
	gcc -o tcpclient src/tcpclient.c

udpserver: src/udpserver.c
	gcc -o udpserver src/udpserver.c

udpclient: src/udpclient.c
	gcc -o udpclient src/udpclient.c

clean:
	rm -f tcpserver tcpclient udpserver udpclient
