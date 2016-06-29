#if (defined(_WIN32) || defined(_WIN64))

#ifndef _IE_PROXY_H_20121224
#define _IE_PROXY_H_20121224

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "../Utility/Lock.h"

using namespace std;

class CIEProxy
{
public:
	CIEProxy();
	int ReadReg();
    int IsOverride(const string & host);
    int SetEnable(int enable);
    int SetHost(const string & host);
    int SetPort(int port);
    int SetPort2(int port);
    int SetOverride(const string & override);
    int SetUser(const string & user);
    int SetPass(const string & pass);
    int SetAuth(const string & auth);

	int GetEnable();
	string GetHost();
	int GetPort();
	string GetOverride();
	string GetUser();
	string GetPass();
	string GetAuth();

private:
	int m_enable;
	string m_host;
	int m_port;
	string m_override;
	vector<string> m_overrideArr;
	string m_user;
	string m_pass;
	string m_auth;
	
	//CLock m_lock;

private:
	int StringLower(string & str);
	int StringMatch(const string & format, const string & str);
};

#endif

#endif

