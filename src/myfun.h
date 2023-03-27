#ifndef __MYFUN_H
#define __MYFUN_H

#define PORT 5555
#define MAXMSG 512

#if defined WIN32
void ClearWinSock();
void myError(int myErrno);
#endif

#endif //__MYFUN_H