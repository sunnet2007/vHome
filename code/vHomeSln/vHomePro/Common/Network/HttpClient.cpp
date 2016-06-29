#include "../Utility/CommFuns.h"
#include "HttpClient.h"
#include "Sock.h"



CHttpClient::CHttpClient()
{
	m_errno = 0;
}

int CHttpClient::AccessUrl(string url, int timeout, int redirectLevel)
{
	if(timeout <= 0)
		timeout = 99999999;

	m_resMsg.Clear();

	string host, path;
	int port = 0;
	if(ParseUrl(url, host, port, path) != 0)
	{
		m_errno = -1;
		return -1;
	}

	//LOG.Log("Access url: %s", url.c_str());

	m_reqMsg.SetHost(host, port);
	m_reqMsg.SetPath(path);

	int64 startTime = GetMilSec();

	string ip;
	if(ntohl(inet_addr(host.c_str())) != 0xffffffff
		&& ntohl(inet_addr(host.c_str())) > ntohl(inet_addr("1.0.0.0")))
	{
		ip = host;
	}
	else
	{
		struct hostent * phe;
		phe = gethostbyname(host.c_str()); 
		if (phe == NULL)
		{
			m_errno = -1;
			return -1;  
		} 
		else
		{
			struct in_addr addr;
			addr.s_addr = *(unsigned long *) phe->h_addr_list[0]; 
			ip = inet_ntoa(addr);
		}
	}

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		m_errno = -2;
		return -1;
	}

	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	servAddr.sin_port = htons(port);

	if(connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
	{
		m_errno = -3;
		CTcpSock::Close(sock);
		return -1;
	}

	//重新计算超时时长
	timeout -= GetMilSec() - startTime;
	if(timeout <= 0)
	{
		m_errno = -4;
		CTcpSock::Close(sock);
		return -1;
	}

	startTime = GetMilSec();

	if(m_reqMsg.SendMsg(sock, timeout) != 0)
	{
		m_errno = -5;
		CTcpSock::Close(sock);
		return -1;
	}

	//重新计算超时时长
	timeout -= GetMilSec() - startTime;
	if(timeout <= 0)
	{
		m_errno = -4;
		CTcpSock::Close(sock);
		return -1;
	}

	string reqCmd;
	m_reqMsg.GetCmd(reqCmd);
	if(reqCmd == "HEAD")
		m_resMsg.SetOnlyHead(1);

	if(m_resMsg.RecvMsg(sock, timeout) != 0)
	{
		m_errno = -6;
		CTcpSock::Close(sock);
		return -1;
	}

	m_errno = 0;
	CTcpSock::Close(sock);

	int stateCode;
	m_resMsg.GetStatusCode(stateCode);
	if(redirectLevel > 0 && (stateCode == 301 || stateCode == 302))
	{
		string location;
		m_resMsg.GetHeader("Location", location);
		if(location.size() > 0)
		{
			string newLocation;
			CvtAbsUrl(url, location, newLocation);
			return AccessUrl(newLocation, timeout, redirectLevel-1);
		}
	}

	return 0;
}

int CHttpClient::PostFile(string url)
{
	string host, path;
	int port = 0;
	if(ParseUrl(url, host, port, path) != 0)
	{
		m_errno = -9;
		return -1;
	}

	//LOG.Log("Access url: %s", url.c_str());

	m_reqMsg.SetHost(host, port);
	m_reqMsg.SetPath(path);

	int64 startTime = GetMilSec();

	int connSucc = 0;
	int sock = -1;
	int i;
	for(i = 0; i < 1; i++)
	{
		sock = CTcpSock::Create();
		if(CTcpSock::Connect(sock, host, port, 3000) == 0)
		{
			connSucc = 1;
			break;
		}
		else
		{
			CTcpSock::Close(sock);
		}
	}

	if(!connSucc)
	{
		m_errno = -1;
		return -1;
	}

	if(m_reqMsg.PostFile(sock) != 0)
	{
		m_errno = -2;
		CTcpSock::Close(sock);
		return -1;
	}

	if(m_resMsg.RecvMsg(sock) != 0)
	{
		m_errno = -2;
		CTcpSock::Close(sock);
		return -1;
	}

	m_errno = 0;
	CTcpSock::Close(sock);

	return 0;
}

int CHttpClient::GetErrno()
{
	return m_errno;
}

int CHttpClient::SetReqCmd(string cmd)
{
	return m_reqMsg.SetCmd(cmd);
}

int CHttpClient::SetReqBody(string &body)
{
    return m_reqMsg.SetBody(body);
}

int CHttpClient::SetReqMultipart(int multipart)
{
	return m_reqMsg.SetMultipart(multipart);
}

int CHttpClient::SetReqArg(string name, string val)
{
	return m_reqMsg.SetArg(name, val);
}

int CHttpClient::SetReqArg(string name, int val)
{
	return m_reqMsg.SetArg(name, val);
}

int CHttpClient::SetReqHeader(string name, string val)
{
	return m_reqMsg.SetHeader(name, val);
}

int CHttpClient::SetReqHeader(string name, int val)
{
	return m_reqMsg.SetHeader(name, val);
}

int CHttpClient::SetPostFile(string arg, string fn)
{
	return m_reqMsg.SetPostFile(arg, fn);
}

int CHttpClient::SetOnlyHead(int onlyHead)
{
	return m_resMsg.SetOnlyHead(onlyHead);
}

int CHttpClient::SetResHeadFun(FPHeadFun bodyFun, void * arg)
{
	return m_resMsg.SetHeadFun(bodyFun, arg);
}

int CHttpClient::SetResBodyFun(FPBodyFun bodyFun, void * arg)
{
	return m_resMsg.SetBodyFun(bodyFun, arg);
}

int CHttpClient::GetResStatusCode()
{
	int code;
	m_resMsg.GetStatusCode(code);

	return code;
}

int CHttpClient::GetResHander(string name, string & val)
{
	return m_resMsg.GetHeader(name, val);
}

int CHttpClient::GetResHead(string & head)
{
	return m_resMsg.GetHead(head);
}

int CHttpClient::GetResBody(string & body)
{
	return m_resMsg.GetBody(body);
}

int CHttpClient::GetResHeadFunRet(int & ret)
{
	return m_resMsg.GetResHeadFunRet(ret);
}

int CHttpClient::GetResBodyFunRet(int & ret)
{
	return m_resMsg.GetResBodyFunRet(ret);
}
