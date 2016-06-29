#ifndef _MSG_PACK_H_20120418
#define _MSG_PACK_H_20120418

#pragma warning(disable : 4786)
#pragma warning(disable : 4005)

#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <time.h>
using namespace std;

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#define snprintf _snprintf
#define socklen_t int
#define int64 __int64
#pragma comment(lib,"ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#define int64 long long
#endif

class CMsgPack {
public:
	CMsgPack();
	~CMsgPack();

	int SetName(string name);
	int GetName(string & name);

	int SetArg(string name, int val);
	int SetArg(string name, int64 val);
	int SetArg(string name, const string & val);
	int SetArg(string name, const char * val);
	int SetArg(string name, const void * val, int byte_num);

	int GetArg(string name, int & val);
	int GetArg(string name, int64 & val);
	int GetArg(string name, string & val);
	int GetArg(string name, void * val, int * byte_num);

	int Encode(string & str);
	int Decode(string & str);

	int SendMsg(int sock, int timeout = 0);
	int RecvMsg(int sock, int timeout = 0);

	void Clear();
	void Print();

private:
	string GenBoundary();

	string m_name;
	map<string, string> m_data;
};

#endif
