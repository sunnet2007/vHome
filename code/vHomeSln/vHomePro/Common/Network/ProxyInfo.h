#if (defined(_WIN32) || defined(_WIN64))

#ifndef _PROXY_INFO_H
#define _PROXY_INFO_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "../Utility/Lock.h"

class CProxyInfo
{
public:
	/*
	static int Init();
	static int ReadIEProxy();
	static bool IsOverride(string host);
	static int SetEnable(int enable);
	static int SetHost(string host);
	static int SetPort(int port);
	static int SetOverride(string override);
	static int SetUser(string user);
	static int SetPass(string pass);
	static int SetAuth(string auth);
	*/

	static int ReadFromReg();
	static int GetCustomProxyEnable();
	static int GetEnable();
	static string GetHost();
	static int GetPort();
	static string GetOverride();
	static string GetUser();
	static string GetPass();
	static string GetAuth();

	static bool IsOverride(string host);

	//static int StringLower(string & str);
	//static int StringMatch(const string & format, const string & str);

public:
	static void Clear();

	static int m_customProxyEnable;
	
	//IE代理的信息
	static int m_ieProxyEnable;
	static string m_ieProxyHost;
	static int m_ieProxyPort;
	static string m_ieProxyOverride;
	static string m_ieProxyUser;
	static string m_ieProxyPass;
	static string m_ieProxyAuth;

	//用户自定义代理信息
	//static int m_customProxyEnable;
	static string m_customProxyHost;
	static int m_customProxyPort;
	static string m_customProxyOverride;
	static string m_customProxyUser;
	static string m_customProxyPass;
	static string m_customProxyAuth;

	static CLock m_lock;
};

#endif

#endif
