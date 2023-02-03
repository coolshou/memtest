#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "version.h"

// #define PORT            5555
// #define MESSAGE         "Yow!!! Are we having fun yet?!?"

#define ERROR_ARG  -1

extern void init_sockaddr (struct sockaddr_in *name,
                            const char *hostname,
                            uint16_t port);
void
write_to_server (int filedes, char *memusage)
{
  int nbytes;
  /*nbytes = write(filedes, MESSAGE, strlen (MESSAGE) + 1);*/
  nbytes = write(filedes, memusage, strlen (memusage) + 1);
  if (nbytes < 0)
    {
      perror ("write");
      exit (EXIT_FAILURE);
    }
}

int converttoint(const char *buff) {
  char *end;
  int si;
 
  errno = 0;
 
  const long sl = strtol(buff, &end, 10);
 
  if (end == buff) {
    fprintf(stderr, "%s: not a decimal number\n", buff);
  } else if ('\0' != *end) {
    fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
  } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
    fprintf(stderr, "%s out of range of type long\n", buff);
  } else if (sl > INT_MAX) {
    fprintf(stderr, "%ld greater than INT_MAX\n", sl);
  } else if (sl < INT_MIN) {
     fprintf(stderr, "%ld less than INT_MIN\n", sl);
  } else {
    si = (int)sl;
    return si;
    /* Process si */
  }
  return -1;
}

void help(char *argv[])
{
  printf("v%s\n", VERSION);
  printf("Usage:\n");
  printf(" %s <ip address> [port] <num of MB>\n", argv[0]);
  printf("\t<ip address>: target ipv4 address\n");
  printf("\t[port]: target port number, default 5555 (option)\n");
  printf("\t<num of MB>: How many memory will consume in MB\n");
}

int
main (int argc, char *argv[])
{
  int sock;
  struct sockaddr_in servername;
  char *ipaddr;
  char *memusage;
  uint16_t port;

  if (argc < 2){
    help(argv);
    exit(ERROR_ARG);
  } 
  ipaddr = argv[1];
  if (argc <= 3){
    port = 5555;
    memusage = argv[2];
  }
  else{
    port = converttoint(argv[2]);
    memusage = argv[3];
  }
  printf("ipaddr:%s, port: %d, memusage:%s\n", ipaddr, port, memusage);

  if (converttoint(memusage)<0){
    exit(ERROR_ARG);
  }  

  /* Create the socket. */
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    {
      perror("socket (client)");
      exit (EXIT_FAILURE);
    }

  /* Connect to the server. */
  init_sockaddr(&servername, ipaddr, port);
  if (0 > connect(sock,
                  (struct sockaddr *) &servername,
                   sizeof (servername)))
    {
      perror("connect (client)");
      exit(EXIT_FAILURE);
    }

  /* Send data to the server. */
  write_to_server(sock, memusage);
  close (sock);
  exit (EXIT_SUCCESS);
}
