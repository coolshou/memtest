#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#if defined _WIN32
#include "win.h"
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include "version.h"
#include "myfun.h"

#define ERROR_ARG -1

extern void init_sockaddr(struct sockaddr_in *name,
                          const char *hostname,
                          uint16_t port);
#if defined _WIN32
extern void ClearWinSock();
extern void myError(int myErrno);
#endif

void write_to_server(int filedes, char *memusage, char *delay, char *wait)
{
  // int stringLen = strlen(memusage);
  int len = strlen(memusage) + strlen(";") + strlen(delay) + strlen(";") + strlen(wait) + 1;
  char *concated = new char[len];
  memset(concated, '\0', len);
  printf("memusage: %s , delay: %s, wait: %s\n", memusage, delay, wait);
  strcat(concated, memusage);
  strcat(concated, ";");
  strcat(concated, delay);
  strcat(concated, ";");
  strcat(concated, wait);

#if defined _WIN32
  if (send(filedes, concated, len, 0) != len)
  {
    fprintf(stderr, "send() sent a different number of bytes than expected");
    // closesocket(Csocket);
    // ClearWinSock();
    exit(EXIT_FAILURE);
  }
#else
  int nbytes;
  nbytes = write(filedes, concated, len);
  // nbytes = write(filedes, memusage, len);
  if (nbytes < 0)
  {
    perror("write");
    exit(EXIT_FAILURE);
  }
#endif
  delete[] concated;
}

int converttoint(const char *buff)
{
  char *end;
  int si;

  errno = 0;

  const long sl = strtol(buff, &end, 10);

  if (end == buff)
  {
    fprintf(stderr, "%s: not a decimal number\n", buff);
  }
  else if ('\0' != *end)
  {
    fprintf(stderr, "%s: extra characters at end of input: %s\n", buff, end);
  }
  else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno)
  {
    fprintf(stderr, "%s out of range of type long\n", buff);
  }
  else if (sl > INT_MAX)
  {
    fprintf(stderr, "%ld greater than INT_MAX\n", sl);
  }
  else if (sl < INT_MIN)
  {
    fprintf(stderr, "%ld less than INT_MIN\n", sl);
  }
  else
  {
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
  printf(" %s -c <ip address> -n <num of MB> -d [delay] -p [port]\n", argv[0]);
  printf("\t-c <ip address>: target ipv4 address\n");
  printf("\t-n <num of MB>: How many memory will consume in MB\n");
  printf("\t-p [port]: target port number, default 5555 (option)\n");
  printf("\t-d [delay]:  delay time on consume memory, default 0 (sec)\n");
  printf("\t-w [wait]:  wait time to free the consume memory, default 30 (sec)\n");
}

int main(int argc, char *argv[])
{
#if defined _WIN32
  SOCKET sock;
#else
  int sock;
#endif
  struct sockaddr_in servername;
  char *ipaddr;
  char *memusage;
  uint16_t iwait = 30;
  uint16_t port = 5555;
  uint16_t delay = 0;
  int rc = 0;
  if (argc < 3)
  {
    help(argv);
    exit(ERROR_ARG);
  }
  int opt;
  while ((opt = getopt(argc, argv, "hp:d:n:c:w:")) != -1)
  {
    switch (opt)
    {
    case 'h':
      help(argv);
      return 0;
    case 'w':
      iwait = converttoint(optarg);
      break;
    case 'p':
      port = converttoint(optarg);
      break;
    case 'd':
      // printf("set delay: %s", optarg);
      delay = converttoint(optarg);
      break;
    case 'n':
      memusage = optarg;
      rc = converttoint(memusage);
      if (rc < 0)
      {
        exit(ERROR_ARG);
      }
      break;
    case 'c':
      ipaddr = optarg;
      break;
    default:
      help(argv);
      exit(ERROR_ARG);
    }
  }

  printf("ipaddr:%s, memusage:%s MB", ipaddr, memusage);
  if (port > 0)
  {
    printf(", port: %d", port);
  }
  if (delay >= 0)
  {
    printf(", delay: %d", delay);
  }
  printf("\n");
#if defined _WIN32
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0)
  {
    printf("error at WSASturtup\n");
    return 0;
  }
  /* Create the socket. */
  // int Csocket;
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
  if (sock < 0)
  {
    fprintf(stderr, "socket creation failed.\n");
    // closesocket(Csocket);
    // ClearWinSock();
    exit(EXIT_FAILURE);
  }
  /* Connect to the server. */
  // Server address construction
  // struct sockaddr_in sad;
  memset(&servername, 0, sizeof(servername));
  servername.sin_family = AF_INET;
  servername.sin_addr.s_addr = inet_addr(ipaddr); // server IP
  servername.sin_port = port;                     // htons(5193);              // Server port
  // Connection to the server
  if (connect(sock, (struct sockaddr *)&servername, sizeof(servername)) < 0)
  {
    int err = WSAGetLastError();
    fprintf(stderr, "Failed to connect.(%d)\n", err);
    // myError(err);
    // closesocket(sock);
    // ClearWinSock();
    exit(EXIT_FAILURE);
  }

  // int stringLen = strlen(memusage);
  // if (send(sock, memusage, stringLen, 0) != stringLen)
  // {
  //   fprintf(stderr, "send() sent a different number of bytes than expected");
  //   // closesocket(Csocket);
  //   // ClearWinSock();
  //   exit(EXIT_FAILURE);
  // }

#else
  /* Create the socket. */
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
  if (sock < 0)
  {
    perror("socket (client)");
    exit(EXIT_FAILURE);
  }
  /* Connect to the server. */
  init_sockaddr(&servername, ipaddr, port);
  if (0 > connect(sock,
                  (struct sockaddr *)&servername,
                  sizeof(servername)))
  {
    perror("connect (client)");
    exit(EXIT_FAILURE);
  }
#endif
  /* Send data to the server. */
  printf("Send data to the server\n");

  char cdelay[2];
  sprintf(cdelay, "%d", delay);
  // printf("cdelay: %s\n", cdelay);
  char cwait[2];
  sprintf(cwait, "%d", iwait);
  write_to_server(sock, memusage, cdelay, cwait);

  close(sock);
#if defined _WIN32
  // ClearWinSock();
#endif
  exit(EXIT_SUCCESS);
}
