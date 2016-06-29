#if (defined(_WIN32) || defined(_WIN64))

#include "stdafx.h"
#include "ProxyInfo.h"
#include "../Utility/StringFuns.h"
#include "../Utility/CommFuns.h"
#include "../Utility/Reg.h"

int CProxyInfo::m_customProxyEnable = 0;

int CProxyInfo::m_ieProxyEnable = 0;
string CProxyInfo::m_ieProxyHost;
int CProxyInfo::m_ieProxyPort = 80;
string CProxyInfo::m_ieProxyOverride = "<local>";
string CProxyInfo::m_ieProxyUser;
string CProxyInfo::m_ieProxyPass;
string CProxyInfo::m_ieProxyAuth;

string CProxyInfo::m_customProxyHost;
int CProxyInfo::m_customProxyPort = 80;
string CProxyInfo::m_customProxyOverride = "<local>";
string CProxyInfo::m_customProxyUser;
string CProxyInfo::m_customProxyPass;
string CProxyInfo::m_customProxyAuth;

CLock CProxyInfo::m_lock;

/*
int CProxyInfo::Init()
{
	CAutoLock autoLock(m_lock);

	m_ieProxyEnable = false;
	m_ieProxyPort = 80;

	return 0;
}

int CProxyInfo::ReadIEProxy()
{
	SetEnable(0);
	SetHost("");
	SetPort(80);
	SetOverride("");

	HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut;
	if( ERROR_SUCCESS != RegOpenKeyEx(hKeyIn, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_CREATE_LINK | KEY_WRITE | KEY_READ | KEY_NOTIFY, &hKeyOut))
	{
		return -1;
	}

	DWORD proxyEnable = 0;
	ULONG regSize = sizeof(DWORD);
	if(ERROR_SUCCESS != RegQueryValueEx(hKeyOut, "ProxyEnable", NULL, NULL, (BYTE *)&proxyEnable, &regSize))
	{
		return 0;
	}

	SetEnable(proxyEnable);

	char proxyServer[1024] = { 0 };
	regSize = sizeof(proxyServer);
	if(ERROR_SUCCESS != RegQueryValueEx(hKeyOut, "ProxyServer", NULL, NULL, (BYTE *)&proxyServer, &regSize))
	{
		return 0;
	}

	string strProxyServer = proxyServer;
	if(strProxyServer.find("://") != string::npos)
	{
		strProxyServer = strProxyServer.substr(strProxyServer.find("://") + 3);
	}

	if(strProxyServer.find(":") != string::npos)
	{
		SetHost(strProxyServer.substr(0, strProxyServer.find(":")));
		SetPort(atoi(strProxyServer.substr(strProxyServer.find(":") + 1).c_str()));
	}
	else
	{
		SetHost(strProxyServer);
		SetPort(80);
	}

	char proxyOverride[1024] = { 0 };
	regSize = sizeof(proxyOverride);
	if(ERROR_SUCCESS == RegQueryValueEx(hKeyOut, "ProxyOverride", NULL, NULL, (BYTE *)&proxyOverride, &regSize))
	{
		SetOverride(proxyOverride);
	}

	return 0;
}

int CProxyInfo::SetEnable(int enable)
{
	CAutoLock autoLock(m_lock);
	m_ieProxyEnable = enable;
	return 0;
}

int CProxyInfo::SetHost(string host)
{
	CAutoLock autoLock(m_lock);
	m_ieProxyHost = host;
	return 0;
}

int CProxyInfo::SetPort(int port)
{
	CAutoLock autoLock(m_lock);
	m_ieProxyPort = port;
	if(m_ieProxyPort <= 0)
		m_ieProxyPort = 80;
	return 0;
}

int CProxyInfo::SetOverride(string override)
{
	CAutoLock autoLock(m_lock);

	m_ieProxyOverride = override;
	m_ieProxyOverrideArr.clear();

	string tmpOverride = override;
	while(tmpOverride.size() > 0)
	{
		string or;
		string::size_type commPos = tmpOverride.find(";");
		if(commPos != string::npos)
		{
			or = tmpOverride.substr(0, commPos);
			tmpOverride = tmpOverride.substr(commPos + 1);
		}
		else
		{
			or = tmpOverride;
			tmpOverride = "";
		}

		while(or.size() > 0)
		{
			if(or[0] == ' ' || or[0] == '\t' || or[0] == '\r' || or[0] == '\n')
				or.erase(0, 1);
			else
				break;
		}

		while(or.size() > 0)
		{
			string::size_type endPos = or.size() - 1;
			if(or[endPos] == ' ' || or[endPos] == '\t' || or[endPos] == '\r' || or[endPos] == '\n')
				or.erase(endPos, 1);
			else
				break;
		}

		if(or.size() > 0)
			m_ieProxyOverrideArr.push_back(or);
	}

	return 0;
}

int CProxyInfo::SetUser(string user)
{
	CAutoLock autoLock(m_lock);
	m_ieProxyUser = user;
	return 0;
}

int CProxyInfo::SetPass(string pass)
{
	CAutoLock autoLock(m_lock);
	m_ieProxyPass = pass;
	return 0;
}

int CProxyInfo::SetAuth(string auth)
{
	CAutoLock autoLock(m_lock);
	m_ieProxyAuth = auth;
	return 0;
}
*/

int CProxyInfo::ReadFromReg()
{
	CAutoLock autoLock(m_lock);
	
	string val;

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "IEProxyEnable", val);
	m_ieProxyEnable = atoi(val.c_str());

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "IEProxyServer", val);
	ParseUrl(val, m_ieProxyHost, m_ieProxyPort);

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "IEProxyOverride", val);
	m_ieProxyOverride = val;
	if(m_ieProxyOverride.size() == 0)
		m_ieProxyOverride = "<local>";

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "IEProxyUser", val);
	m_ieProxyUser = val;

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "IEProxyPass", val);
	m_ieProxyPass = val;

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "IEProxyAuth", val);
	m_ieProxyAuth = val;

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "CustomProxyEnable", val);
	m_customProxyEnable = atoi(val.c_str()) ;

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "CustomProxyServer", val);
	ParseUrl(val, m_customProxyHost, m_customProxyPort);

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "CustomProxyOverride", val);
	m_customProxyOverride = val;
	if(m_customProxyOverride.size() == 0)
		m_customProxyOverride = "<local>";

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "CustomProxyUser", val);
	m_customProxyUser = val;

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "CustomProxyPass", val);
	m_customProxyPass = val;

	GetRegValue_S(HKEY_LOCAL_MACHINE, "Software\\NEWV\\VDPPLAYER\\ProxyInfo", "CustomProxyAuth", val);
	m_customProxyAuth = val;

	return 0;
}

int CProxyInfo::GetCustomProxyEnable()
{
	CAutoLock autoLock(m_lock);

	return m_customProxyEnable;
}

int CProxyInfo::GetEnable()
{
	CAutoLock autoLock(m_lock);
	
	if(m_customProxyEnable)
		return 1;
	
	return m_ieProxyEnable;
}

string CProxyInfo::GetHost()
{
	CAutoLock autoLock(m_lock);

	if(m_customProxyEnable)
	{
		return m_customProxyHost;
	}
	else
	{
		return m_ieProxyHost;
	}
}

int CProxyInfo::GetPort()
{
	CAutoLock autoLock(m_lock);

	if(m_customProxyEnable)
	{
		return m_customProxyPort;
	}
	else
	{
		return m_ieProxyPort;
	}
}

string CProxyInfo::GetOverride()
{
	CAutoLock autoLock(m_lock);

	if(m_customProxyEnable)
	{
		return m_customProxyOverride;
	}
	else
	{
		return m_ieProxyOverride;
	}
}

string CProxyInfo::GetUser()
{
	CAutoLock autoLock(m_lock);

	if(m_customProxyEnable)
	{
		return m_customProxyUser;
	}
	else
	{
		return m_ieProxyUser;
	}
}

string CProxyInfo::GetPass()
{
	CAutoLock autoLock(m_lock);

	if(m_customProxyEnable)
	{
		return m_customProxyPass;
	}
	else
	{
		return m_ieProxyPass;
	}
}

string CProxyInfo::GetAuth()
{
	CAutoLock autoLock(m_lock);

	if(m_customProxyEnable)
	{
		return m_customProxyAuth;
	}
	else
	{
		return m_ieProxyAuth;
	}
}

bool CProxyInfo::IsOverride(string host)
{
	CAutoLock autoLock(m_lock);

	if(!m_customProxyEnable)
	{
		if(!m_ieProxyEnable)
			return true;
	}

	if(stricmp(host.c_str(), "localhost") == 0 || host == "::1")
		return true;

	if(ntohl(inet_addr(host.c_str())) != 0xffffffff)
	{
		if(ntohl(inet_addr(host.c_str())) >= ntohl(inet_addr("127.0.0.1"))
			&& ntohl(inet_addr(host.c_str())) <= ntohl(inet_addr("127.255.255.255")))
			return true;
	}

	static vector<string> overrideArr;
	if(m_customProxyEnable)
	{
		StringSplit(m_customProxyOverride, overrideArr, ';');
	}
	else
	{
		StringSplit(m_ieProxyOverride, overrideArr, ';');
	}

	int i;
	for(i = 0; i < overrideArr.size(); i++)
	{
		string override = overrideArr[i];
		StringTrim(override);
		if(override.size() == 0)
			continue;

		if(stricmp(override.c_str(), "<local>") == 0)
		{
			#if 0
			//解析后判断局域网
			string ip;
			if(ntohl(inet_addr(host.c_str())) != 0xffffffff)
			{
				ip = host;
			}
			else
			{
				struct hostent * phe;
				phe = gethostbyname(host.c_str()); 
				if(phe == NULL)
				{
					continue;
				} 
				else
				{
					struct in_addr addr;
					addr.s_addr = *(unsigned long *) phe->h_addr_list[0]; 
					ip = inet_ntoa(addr);
				}
			}
			#endif

			if(ntohl(inet_addr(host.c_str())) != 0xffffffff)
			{
				if(ntohl(inet_addr(host.c_str())) >= ntohl(inet_addr("10.0.0.0"))
					&& ntohl(inet_addr(host.c_str())) <= ntohl(inet_addr("10.255.255.255")))
					return true;

				if(ntohl(inet_addr(host.c_str())) >= ntohl(inet_addr("192.168.0.0"))
					&& ntohl(inet_addr(host.c_str())) <= ntohl(inet_addr("192.168.255.255")))
					return true;

				if(ntohl(inet_addr(host.c_str())) >= ntohl(inet_addr("172.16.0.0"))
					&& ntohl(inet_addr(host.c_str())) <= ntohl(inet_addr("172.31.255.255")))
					return true;
			}
		}
		else
		{
			//模板匹配
			string tmpHost = host;
			StringLower(tmpHost);
			if(StringWildcardMatch(override, tmpHost))
				return true;
		}
	}

	return false;
}

void CProxyInfo::Clear()
{
	m_customProxyEnable = 0;

	m_ieProxyEnable = 0;
	m_ieProxyHost = "";
	m_ieProxyPort = 80;
	m_ieProxyOverride = "<local>";
	m_ieProxyUser = "";
	m_ieProxyPass = "";
	m_ieProxyAuth = "";

	m_customProxyHost = "";
	m_customProxyPort = 80;
	m_customProxyOverride = "<local>";
	m_customProxyUser = "";
	m_customProxyPass = "";
	m_customProxyAuth = "";
}

/*
int CProxyInfo::StringLower(string & str)
{
	int i;
	for(i = 0; i < str.size(); i++)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}

	return 0;
}

//返回1匹配成功，返回0匹配失败
int CProxyInfo::StringMatch(const string & format, const string & str)  
{  
	int formatLen = format.size();
	int strLen = str.size();

	if(formatLen == 0 && strLen == 0)
		return 1;

	//实际使用时根据strl的长度来动态分配表的内存
	char matchmap[128][128];
	memset(matchmap, 0, 128*128); 
	matchmap[0][0] = 1;  
	int i, j, k;  
	//遍历目标字符串符串
	for(i = 1; i<= strLen; ++i)  
	{  
		//遍历通配符串
		for(j = 1; j<=formatLen; ++j)
		{
			//当前字符之前的字符是否已经得到匹配
			if(matchmap[i-1][j-1])
			{
				//匹配当前字符
				if(str[i-1] == format[j-1] || format[j-1] == '?')
				{ 
					matchmap[i][j] = 1; 
					//考虑星号在末尾的情况
					if( i  == strLen && j < formatLen)
					{
						for ( k = j+1 ; k <= formatLen ; ++k )
						{
							if( '*' == format[k-1])
							{
								matchmap[i][k] = 1;
							}
							else
							{
								break;
							}
						}
					}
				}
				else if(format[j-1] == '*')
				{
					//遇到星号，目标字符串到末尾都能得到匹配
					for(k = i-1; k<=strLen; ++k)
					{
						matchmap[k][j] = 1;  
					}
				}
			}
		}
		//如果当前字符得到了匹配则继续循环，否则匹配失败
		for(k = 1; k<=formatLen; ++k)  
		{
			if(matchmap[i][k])
			{
				break; 
			}
		}
		if(k>formatLen)
		{
			return 0;  
		}
	}

	return (int) matchmap[strLen][formatLen];  
} 
*/

#endif
