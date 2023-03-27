#ifndef __WIN_H
#define __WIN_H

#if defined _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x601
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
// #include <er
#include "myfun.h"

typedef unsigned short uint16_t;

// #pragma comment(lib, "ws2_32.lib") // Winsock Library, not support by mingw64!! only MSVC

#endif

#endif //__WIN_H