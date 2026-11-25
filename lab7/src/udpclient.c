#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
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

  int sockfd, n;
  char sendline[bufsize], recvline[bufsize + 1];
  struct sockaddr_in servaddr;


  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);

  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  write(1, "Enter string\n", 13);

  while ((n = read(0, sendline, bufsize)) > 0) {
    if (sendto(sockfd, sendline, n, 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
      perror("sendto problem");
      exit(1);
    }

    n = recvfrom(sockfd, recvline, bufsize, 0, NULL, NULL);
    if (n == -1) {
      perror("recvfrom problem");
      exit(1);
    }

    recvline[n] = '\0';
    printf("REPLY FROM SERVER= %s\n", recvline);
  }
  close(sockfd);
}
