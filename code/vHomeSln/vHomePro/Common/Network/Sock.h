#ifndef _SOCK_H_20120522
#define _SOCK_H_20120522

#pragma warning(disable : 4786)
#pragma warning(disable : 4005)

#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <time.h>
#include "NetAddr.h"
using namespace std;

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#define snprintf _snprintf
#define socklen_t int
#define int64 __int64
#pragma comment(lib,"ws2_32.lib")
#else
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#define int64 long long
#define closesocket close
#define GetLastError() errno
#define Sleep(x) usleep((x)*1000)
#endif

int RecvFromSock(int sock, void * buf, int len, int timeout = 0);

int SendToSock(int sock, const void * buf, int len, int timeout = 0);

bool IsBrokenSock(int err);

int SetSockNonBlock(int sock);

class CTcpSock
{
public:
	static int Create(); // return a socket handle on success
	static int Close(int sock);
	static int Connect(int sock, string host, int port, int timeout = 1000 /* millisecond */); // return 0 on success, -1 on failure
	static int Send(int sock, const void * data, int length);
	static int Send(int sock, const string & data);
	static int Recv(int sock, void * data, int * length, int timeout = 10 /* millisecond */);
	static int Listen(int sock, const string ip, int port);
	static int Listen(int sock, int port);
	static int Accept(int sock,CNetAddr &addr);
	static int Accept(int sock, string & ip, int & port); // return client socket handle on success
	static int Accept(int sock);
	static int CanRead(int sock);
	static int CanWrite(int sock);
};

/*
class CUDPMsg
{
public:
	CUDPMsg()
	{
		sock = -1;
		port = -1;
	}

	void Clear()
	{
		data = "";
		sock = -1;
		IP = "";
		port = -1;
	}

	int sock;
	string IP;
	int port;
	string data;
};

class CUdpSock
{
public:
	static int Create(); // return a socket handle on success
	static int Close(int sock);
	static int Bind(int sock, int port);
	static int Bind(int sock, const string & ip, int port);
	static int SendTo(int sock, const void * buf, int len, const string & descAddr, int descPort);
	static int SendTo(int sock, CUDPMsg & msg);
	static int RecvFrom(int sock, void * buf, int len, string & srcIP, int & srcPort, int timeout = -1);
	static int RecvFrom(int sock, void * buf, int len, int timeout = -1);
	static int RecvFrom(int sock, CUDPMsg & msg, int timeout = -1);
	static int CanRead(int sock);
	static int CanWrite(int sock);
};
*/

#endif

