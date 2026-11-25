#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <server_ip> <port> <buffer_size>\n", argv[0]);
    return 1;
  }

  char *end;
  long port = strtol(argv[2], &end, 10);
  if (*end || port < 1 || port > 65535) {
    fprintf(stderr, "Port must be between 1 and 65535\n");
    return 1;
  }
  long bufsize = strtol(argv[3], &end, 10);
  if (*end || bufsize < 1 || bufsize > 65536) {
    fprintf(stderr, "Buffer size must be between 1 and 65536 bytes\n");
    return 1;
  }

  int fd;
  int nread;
  char buf[bufsize];
  struct sockaddr_in servaddr;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(port);

  if (connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, bufsize)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
