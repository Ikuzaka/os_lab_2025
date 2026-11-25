CC ?= cc
CFLAGS ?= -O2 -Wall -Wextra

PROGRAMS = tcpserver tcpclient udpserver udpclient

.PHONY: all clean

all: $(PROGRAMS)

$(PROGRAMS): %: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

clean:
	rm -f $(PROGRAMS)
