#include <stdio.h>
#include <stdlib.h>

#if defined _WIN32
#include "win.h"
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <string>

int make_socket(uint16_t port)
{

#if defined _WIN32
  struct addrinfo *result = NULL;
  // struct addrinfo *ptr = NULL;
  struct addrinfo hints;

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve the local address and port to be used by the server
  std::string sport = std::to_string(port);
  const char *char_port = sport.c_str();

  int iResult = getaddrinfo(NULL, char_port, &hints, &result);
  if (iResult != 0)
  {
    printf("getaddrinfo failed: %d\n", iResult);
    WSACleanup();
    exit(EXIT_FAILURE);
  }
  SOCKET ListenSocket = INVALID_SOCKET;
  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

  if (ListenSocket == INVALID_SOCKET)
  {
    printf("Error at socket(): %d\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    exit(EXIT_FAILURE);
  }
  iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
  if (iResult == SOCKET_ERROR)
  {
    printf("bind failed with error: %d\n", WSAGetLastError());
    freeaddrinfo(result);
    closesocket(ListenSocket);
    WSACleanup();
    exit(EXIT_FAILURE);
  }
  return ListenSocket;
#else

  int sock;
  struct sockaddr_in name;

  /* Create the socket. */
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
  if (sock < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /* Give the socket a name. */
  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  return sock;
#endif
}
