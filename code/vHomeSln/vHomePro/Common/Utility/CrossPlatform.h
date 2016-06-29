#ifndef _CROSS_PLATFORM_H
#define _CROSS_PLATFORM_H

#if (defined(_WIN32) || defined(_WIN64))

#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define int64               __int64
#define snprintf            _snprintf
#define vsnprintf           _vsnprintf
#define strcasecmp          stricmp
#define strncasecmp         strnicmp
#define strcasestr          stristr
#define sleepms(x)          Sleep(x)
#define sleep(x)            Sleep((x)*1000)
#define usleep(x)           Sleep((x)/1000+1)
#define close               closesocket
#define socklen_t           int
#define getpid				GetCurrentProcessId
//#define errno				GetLastError()

HANDLE StartThread(LPTHREAD_START_ROUTINE fun, void * arg);

char *stristr(const char *String, const char *Pattern);

void EndThread(int exitCode);

#pragma comment(lib,"ws2_32.lib")

#else

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>

#define CONST               const
#define int64               long long
#define __int64             long long
#define DWORD               unsigned long
#define BOOL                int
#define BYTE                unsigned char
#define WORD                unsigned short
#define FLOAT               float
#define INT                 int
#define UINT                unsigned int

#define _snprintf           snprintf
#define _vsnprintf          vsnprintf
#define stricmp             strcasecmp
#define strnicmp            strncasecmp
#define stristr             strcasestr
#define sleepms(x)          usleep((x)*1000)
#define Sleep(x)            usleep((x)*1000)
#define closesocket         close
#define SOCKET              int
#define GetCurrentProcessId	getpid
#define GetLastError()		errno

#define __stdcall  __attribute__((__stdcall__))
#define __cdecl    __attribute__((__cdecl__))
#define __fastcall __attribute__((__fastcall__))
#define __thiscall

#define _stdcall   __stdcall
#define _cdecl     __cdecl
#define _fastcall  __fastcall
#define _thiscall

typedef void * (*LPTHREAD_START_ROUTINE)(void *);

pthread_t StartThread(LPTHREAD_START_ROUTINE fun, void * arg);

int MakeBackground();
void EndThread(int exitCode);

#endif

int InitSock();

#define _StartThread(fun, arg) StartThread((LPTHREAD_START_ROUTINE)fun, arg)

int MakeBackground();
#define _EndThread(exitcode)    EndThread(exitcode)

#endif
