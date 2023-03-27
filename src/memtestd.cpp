#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#if defined _WIN32
#include "win.h"
#else
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
#include <sys/types.h>

#include "myfun.h"

extern int memeat(int memusage, int iwait, int delay);
extern int make_socket(uint16_t port);

void split(char **arr, char *str, const char *del)
{
  char *s = strtok(str, del);

  while (s != NULL)
  {
    *arr++ = s;
    s = strtok(NULL, del);
  }
}

int read_from_client(int filedes)
{
  char buffer[MAXMSG];
  int nbytes;
  char *arr[3];

  nbytes = read(filedes, buffer, MAXMSG);
  if (nbytes < 0)
  {
    /* Read error. */
    perror("read");
    exit(EXIT_FAILURE);
  }
  else if (nbytes == 0)
    /* End-of-file. */
    return -1;
  else
  {
    /* Data read. */
    fprintf(stderr, "Server: got message: '%s'\n", buffer);
    if (std::strchr(buffer, ';') != nullptr)
    {
      split(arr, buffer, ";");
      memeat(atoi(arr[0]), atoi(arr[2]), atoi(arr[1]));
    }
    else
    {
      memeat(atoi(buffer), 30, 0);
    }
    // pch = strchr(buffer, ';');
    // memuse;delay
    // memeat(atoi(buffer), 30, 1);
    // memeat(atoi(buffer), 30);
    return 0;
  }
}

int main(void)
{
#if defined _WIN32
  SOCKET sock;
#else
  int sock;
#endif
  fd_set active_fd_set, read_fd_set;
  int i;
  struct sockaddr_in clientname;
  /*size_t size;*/
  socklen_t size;
#if defined _WIN32
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0)
  {
    printf("error at WSASturtup\n");
    exit(EXIT_FAILURE);
  }
#endif
  /* Create the socket and set it up to accept connections. */
  sock = make_socket(PORT);
  if (listen(sock, 1) < 0)
  {
    perror("listen Fail");
    exit(EXIT_FAILURE);
  }

  /* Initialize the set of active sockets. */
  FD_ZERO(&active_fd_set);
  FD_SET(sock, &active_fd_set);

  printf("Wait for client...\n");
  while (1)
  {
    /* Block until input arrives on one or more active sockets. */
    read_fd_set = active_fd_set;
    if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
    {
      perror("select");
      exit(EXIT_FAILURE);
    }

    /* Service all the sockets with input pending. */
    for (i = 0; i < FD_SETSIZE; ++i)
      if (FD_ISSET(i, &read_fd_set))
      {
        if (i == sock)
        {
          /* Connection request on original socket. */
          int inew;
          size = sizeof(clientname);
          inew = accept(sock,
                        (struct sockaddr *)&clientname,
                        &size);
          if (inew < 0)
          {
            perror("accept");
            exit(EXIT_FAILURE);
          }
          fprintf(stderr,
                  "Server: connect from host %s, port %hd.\n",
                  inet_ntoa(clientname.sin_addr),
                  ntohs(clientname.sin_port));
          FD_SET(inew, &active_fd_set);
        }
        else
        {
          /* Data arriving on an already-connected socket. */
          if (read_from_client(i) < 0)
          {
            close(i);
            FD_CLR(i, &active_fd_set);
          }
        }
      }
  }
}
