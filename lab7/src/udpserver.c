#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <port> <buffer_size>\n", argv[0]);
    return 1;
  }

  char *end;
  long port = strtol(argv[1], &end, 10);
  if (*end || port < 1 || port > 65535) {
    fprintf(stderr, "Port must be between 1 and 65535\n");
    return 1;
  }
  long bufsize = strtol(argv[2], &end, 10);
  if (*end || bufsize < 1 || bufsize > 65536) {
    fprintf(stderr, "Buffer size must be between 1 and 65536 bytes\n");
    return 1;
  }

  int sockfd, n;
  char mesg[bufsize + 1], ipadr[16];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("SERVER starts...\n");

  while (1) {
    socklen_t len = sizeof(cliaddr);

    if ((n = recvfrom(sockfd, mesg, bufsize, 0, (struct sockaddr *)&cliaddr, &len)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    mesg[n] = 0;

    printf("REQUEST %s      FROM %s : %d\n", mesg,
           inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
           ntohs(cliaddr.sin_port));

    if (sendto(sockfd, mesg, n, 0, (struct sockaddr *)&cliaddr, len) < 0) {
      perror("sendto");
      exit(1);
    }
  }
}
