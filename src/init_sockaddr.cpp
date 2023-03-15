#include <stdio.h>
#include <stdlib.h>
#if defined _WIN32
#include "win.h"
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#ifndef __USE_MISC
#define __USE_MISC
#endif

void init_sockaddr(struct sockaddr_in *name,
                   const char *hostname,
                   uint16_t port)
{
  struct hostent *hostinfo;

  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  hostinfo = gethostbyname(hostname);
  if (hostinfo == NULL)
  {
    perror("gethostbyname");
    exit(EXIT_FAILURE);
  }
  /* name->sin_addr = *(struct in_addr *) hostinfo->h_addr;*/
  name->sin_addr = *(struct in_addr *)hostinfo->h_addr_list[0];
}
