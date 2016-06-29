#ifndef _SEND_MAIL_H
#define _SEND_MAIL_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

#if (defined(_WIN32) || defined(_WIN64))

#include <windows.h>
#include <time.h>

#define snprintf   _snprintf
#define vsnprintf  _vsnprintf
#define strcasecmp stricmp
#define strcasestr stristr

#define int64 __int64

#pragma warning(disable : 4786)
#pragma comment(lib,"ws2_32.lib")

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>

#define int64 long long
#define closesocket         close
#define SOCKET              int

#endif

class CMailInfo
{
public:
	CMailInfo()
	{
		port = 25;
	}
	string server;
	int    port;
	string user;
	string pass;
	string from;
	vector<string> to;
	string subject;
	string body;
};

int SendMail(CMailInfo mailInfo);

#endif
