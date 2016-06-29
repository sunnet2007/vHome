#ifndef _UDP_SOCK_H
#define _UDP_SOCK_H

#include <time.h>
#include <iostream>
using namespace std;
#include "../Utility/CrossPlatform.h"
#include "MsgPack.h"

class CUDPHelper
{
public:
	static int CreateSock();
	static int CloseSock(int sock);
	static int Bind(int sock, int port);
	static int Bind(int sock, const string & ip, int port);
	static int SendTo(int sock, const void * buf, int len, const string & descAddr, int descPort);
	static int RecvFrom(int sock, void * buf, int len, string & srcIP, int & srcPort, int timeout = -1);
	static int RecvFrom(int sock, void * buf, int len, int timeout = -1);

	static int SendTo(int sock, CMsgPack args, const string & descAddr, int descPort);
	static int RecvFrom(int sock, CMsgPack & args, string & srcIP, int & srcPort, int timeout = -1);
	static int RecvFrom(int sock, CMsgPack & args, int timeout = -1);

	static int RecvFrom(int sock, void * buf, int len, sockaddr_in &addr,int timeout = -1);
	static int SendTo(int sock, void * buf, int len, sockaddr_in &addr);
};

#endif
