#ifndef _HTTP_CLIENT_H
#define _HTTP_CLIENT_H

#include "HttpMsg.h"
//#include "Lock.h"

class CHttpClient
{
public:
	CHttpClient();
	int AccessUrl(string url, int timeout = 180000 /* ms */, int redirectLevel = 10);
	int PostFile(string url);
	int GetErrno();
	int SetReqCmd(string cmd);
    int SetReqBody(string &body);
	int SetReqMultipart(int multipart);
	int SetReqArg(string name, string val);
	int SetReqArg(string name, int val);
	int SetReqHeader(string name, string val);
	int SetReqHeader(string name, int val);
	int SetPostFile(string arg, string fn);
	int SetOnlyHead(int onlyHead);
	int SetResHeadFun(FPHeadFun headFun, void * arg);
	int SetResBodyFun(FPBodyFun bodyFun, void * arg);
	int GetResStatusCode();
	int GetResHander(string name, string & val);
	int GetResHead(string & head);
	int GetResBody(string & body);
	int GetResHeadFunRet(int & ret);
	int GetResBodyFunRet(int & ret);

private:
	CHttpRequestMsg m_reqMsg;
	CHttpResponseMsg m_resMsg;
	int m_errno;
    string m_errMsg;
};

#endif
