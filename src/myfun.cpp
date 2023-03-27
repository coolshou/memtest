
#if defined WIN32
#include "myfun.h"

void ClearWinSock()
{
    WSACleanup();
}
void myError(int myErrno)
{
    // int  err;
    char msgbuf[256]; // for a message up to 255 bytes.

    msgbuf[0] = '\0'; // Microsoft doesn't guarantee this on man page.

    // err = WSAGetLastError();

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // flags
                  NULL,                                                       // lpsource
                  myErrno,                                                    // message id
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),                  // languageid
                  msgbuf,                                                     // output buffer
                  sizeof(msgbuf),                                             // size of msgbuf, bytes
                  NULL);                                                      // va_list of arguments

    if (!*msgbuf)
        sprintf(msgbuf, "%d", err); // provide error # if no string available
}
#endif