#ifndef _HTTP_MSG_H_20120717
#define _HTTP_MSG_H_20120717

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#define snprintf _snprintf
#define socklen_t int
#define int64 __int64
#define snprintf   _snprintf
#define vsnprintf  _vsnprintf
#define strcasecmp stricmp
#define strncasecmp strnicmp
#define strcasestr stristr
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
#include <errno.h>
#define int64 long long
#define closesocket close
#define Sleep(x) usleep((x)*1000)
#define strnicmp strncasecmp
#endif

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

int GetHTTPRequestCmd(string head, string & cmd);

int GetHTTPStatusCode(string head, string & code);

int SetHTTPStatusCode(string & head, string code);

int GetHTTPPath(string head, string & path);

int SetHTTPPath(string & head, string path);

int GetHTTPHeader(string head, string name, string & value);

int SetHTTPHeader(string & head, string name, string value);

int RemoveHTTPHeader(string & head, string name);

int GetHTTPHeadLength(string head);

int RecvHTTPRequestHead(int sock, string & head, int timeout = -1);

int RecvHTTPResponseHead(int sock, string & head, int timeout = -1);

enum REQ_METHOD
{
	METHOD_GET,
	METHOD_HEAD,
	METHOD_POST,
	METHOD_POST_MULTIPART
};

class CFormArgs
{
public:
	CFormArgs();

	int GetFormArgsFromEnv();
	int SetArg(const string & argname, string argvalue);
	int GetArg(const string & argname, string & argvalue);
	int SetArg(const string & argname, int64 argvalue);
	int GetArg(const string & argname, int64 & argvalue);
	int GetArg(const string & argname, int & argvalue);
	string GetBoundary();
	int Dump(string & str);
	int Encode(string & str, REQ_METHOD reqMethod = METHOD_GET);
	int Decode(const string & args, string method, int type = -1); // type, 0 for simple, 1 from multipart, -1 for auto
	void Clear();
private:
	string EncodeVal(string arg);

	string m_boundary;
	map<string, string> m_args;
};

class CHttpRequestMsg
{
public:
	CHttpRequestMsg();
	~CHttpRequestMsg();

	int SetCmd(const string & cmd);
	int SetPath(const string & path);
	int SetArg(const string & argname, string argvalue);
	int SetArg(const string & argname, int64 argvalue);
	int SetProtocol(const string & version);
	int SetHeader(const string & headerName, const string & headerValue);
	int SetHeader(const string & headerName, int headerValue);
	int SetPostFile(const string & postFileArg, const string & postFn);
	int SetBody(const string & body);
	int SetHost(const string & host, int port = 80);
	int SetMultipart(int multipart);
	int SetProxy(bool useProxy, const string & auth = "", const string & user = "", const string & pass = "");

	int GetPath(string & path);
	int GetFullPath(string & path); //GET中，带参数的path。如，/a.php?id=32&t=332
	int GetArg(const string & argname, string & argvalue);
	int GetArg(const string & argname, int & argvalue);
	int GetArg(const string & argname, int64 & argvalue);
	int GetArgsStr(string & argsStr);
	int GetCmd(string & m_cmd);
	int GetProtocol(string & version);
	int GetHeader(const string & headerName, string & headerValue);
	int GetHeader(int idx, string & headerName, string & headerValue);
	int GetBody(string & body);

	int Encode(string & s);
	int Decode(const string & s);
	
	int SendMsg(int sock, int timeout = 0);
	int RecvMsg(int sock, int timeout = 0);
	int PostFile(int sock);

	string Base64Encode(const string & data);
	
	int Clear();

private:
	string m_cmd;
	string m_path;
	map<string, string> m_headers;
	CFormArgs m_formArgs;
	string m_formArgsStr;
	string m_postFileArg;
	string m_postFn;
	string m_body;
	string m_version;
	string m_host;
	int m_port;
	int m_multipart;
	bool m_useProxy;
	string m_proxyAuth;
	string m_proxyUser;
	string m_proxyPass;
};

typedef int (*FPHeadFun)(void * arg, const char * buf, int len);
typedef int (*FPBodyFun)(void * arg, const char * type, int fileLength, int offset, const char * buf, int len);

class CHttpResponseMsg
{
public:
	CHttpResponseMsg();
	~CHttpResponseMsg();

	int SetStatus(int code, const string & desc);
	int SetStatusCode(int code);
	int SetStatusDesc(const string & desc);
	int SetProtocol(const string & version);
	int SetHeader(const string & headerName, const string & headerValue);
	int SetHeader(const string & headerName, int headerValue);
	int SetBody(const string & body);
	int SetOnlyHead(int onlyHead);
	int SetHeadFun(FPHeadFun fun, void * arg);
	int SetBodyFun(FPBodyFun fun, void * arg);
	int AppendBody(const string & body);

	int GetStatus(int & code, string & desc);
	int GetStatusCode(int & code);
	int GetStatusDesc(string & desc);
	int GetProtocol(string & version);
	int GetHeader(const string & headerName, string & headerValue);
	int GetHeader(int idx, string & headerName, string & headerValue);
	int GetHead(string & head);
	int GetBody(string & body);
	int GetResHeadFunRet(int & ret);
	int GetResBodyFunRet(int & ret);

	int Encode(string & s);
	int Decode(const string & s);

	int SendMsg(int sock, int timeout = 0);
	int RecvMsg(int sock, int timeout = 0);

	int Clear();

private:
	string m_statusCode;
	string m_statusDesc;
	string m_version;
	map<string, string> m_headers;
	string m_head;
	string m_body;
	int m_onlyHead;
	FPHeadFun m_headFun;
	void * m_headFunArg;
	FPBodyFun m_bodyFun;
	void * m_bodyFunArg;
	int m_resHeadFunCalled;
	int m_resBodyFunCalled;
	int m_resHeadFunRet;
	int m_resBodyFunRet;
};

#endif
