#if (defined(_WIN32) || defined(_WIN64))

#include "stdafx.h"
#include "IEProxy.h"

CIEProxy::CIEProxy()
{
	m_enable = false;
	m_port = 80;
}

int CIEProxy::ReadReg()
{
	SetEnable(0);
	SetHost("");
	SetPort2(-1);
	SetOverride("");

	HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut;
	if( ERROR_SUCCESS != RegOpenKeyExA(hKeyIn, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_CREATE_LINK | KEY_WRITE | KEY_READ | KEY_NOTIFY, &hKeyOut))
	{
		return -1;
	}
	
	DWORD proxyEnable = 0;
	ULONG regSize = sizeof(DWORD);
	if(ERROR_SUCCESS != RegQueryValueExA(hKeyOut, "ProxyEnable", NULL, NULL, (BYTE *)&proxyEnable, &regSize))
	{
		return 0;
	}

	SetEnable(proxyEnable);

	char proxyServer[1024] = { 0 };
	regSize = sizeof(proxyServer);
	if(ERROR_SUCCESS != RegQueryValueExA(hKeyOut, "ProxyServer", NULL, NULL, (BYTE *)&proxyServer, &regSize))
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
		SetPort2(atoi(strProxyServer.substr(strProxyServer.find(":") + 1).c_str()));
	}
	else
	{
		SetHost(strProxyServer);
		SetPort2(80);
	}

	char proxyOverride[1024] = { 0 };
	regSize = sizeof(proxyOverride);
	if(ERROR_SUCCESS == RegQueryValueExA(hKeyOut, "ProxyOverride", NULL, NULL, (BYTE *)&proxyOverride, &regSize))
	{
		SetOverride(proxyOverride);
	}

	return 0;
}

int CIEProxy::SetEnable(int enable)
{
	//CAutoLock autoLock(m_lock);
	m_enable = enable;
	return 0;
}

int CIEProxy::SetHost(const string & host)
{
	//CAutoLock autoLock(m_lock);
	m_host = host;
	return 0;
}

int CIEProxy::SetPort(int port)
{
    //CAutoLock autoLock(m_lock);
    m_port = port;
    if(m_port == 0)
        m_port = 80;
    return 0;
}

int CIEProxy::SetPort2(int port)
{
	//CAutoLock autoLock(m_lock);
	m_port = port;
	if(m_port == 0)
		m_port = 80;
	return 0;
}

int CIEProxy::SetOverride(const string & override)
{
	//CAutoLock autoLock(m_lock);

	m_override = override;
	m_overrideArr.clear();

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
			m_overrideArr.push_back(or);
	}

	return 0;
}

int CIEProxy::SetUser(const string & user)
{
	//CAutoLock autoLock(m_lock);
	m_user = user;
	return 0;
}

int CIEProxy::SetPass(const string & pass)
{
	//CAutoLock autoLock(m_lock);
	m_pass = pass;
	return 0;
}

int CIEProxy::SetAuth(const string & auth)
{
	//CAutoLock autoLock(m_lock);
	m_auth = auth;
	return 0;
}

int CIEProxy::GetEnable()
{
	//CAutoLock autoLock(m_lock);
	return m_enable;
}

string CIEProxy::GetHost()
{
	//CAutoLock autoLock(m_lock);
	return m_host;
}

int CIEProxy::GetPort()
{
	//CAutoLock autoLock(m_lock);
	return m_port;
}

string CIEProxy::GetOverride()
{
	//CAutoLock autoLock(m_lock);
	return m_override;
}

int CIEProxy::IsOverride(const string & host)
{
	//CAutoLock autoLock(m_lock);

	if(!m_enable)
		return 1;

	if(stricmp(host.c_str(), "localhost") == 0 || host == "::1")
		return 1;

	if(ntohl(inet_addr(host.c_str())) != 0xffffffff)
	{
		if(ntohl(inet_addr(host.c_str())) >= ntohl(inet_addr("127.0.0.1"))
			&& ntohl(inet_addr(host.c_str())) <= ntohl(inet_addr("127.255.255.255")))
			return 1;
	}

	int i;
	for(i = 0; i < m_overrideArr.size(); i++)
	{
		if(stricmp(m_overrideArr[i].c_str(), "<local>") == 0)
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
					return 1;

				if(ntohl(inet_addr(host.c_str())) >= ntohl(inet_addr("192.168.0.0"))
					&& ntohl(inet_addr(host.c_str())) <= ntohl(inet_addr("192.168.255.255")))
					return 1;

				if(ntohl(inet_addr(host.c_str())) >= ntohl(inet_addr("172.16.0.0"))
					&& ntohl(inet_addr(host.c_str())) <= ntohl(inet_addr("172.31.255.255")))
					return 1;
			}
		}
		else
		{
			//模板匹配
			string tmpOverride = m_overrideArr[i];
			string tmpHost = host;
			StringLower(tmpOverride);
			StringLower(tmpHost);
			if(StringMatch(tmpOverride, tmpHost))
				return 1;
		}
	}

	return 0;
}

string CIEProxy::GetUser()
{
	//CAutoLock autoLock(m_lock);
	return m_user;
}

string CIEProxy::GetPass()
{
	//CAutoLock autoLock(m_lock);
	return m_pass;
}

string CIEProxy::GetAuth()
{
	//CAutoLock autoLock(m_lock);
	return m_auth;
}

int CIEProxy::StringLower(string & str)
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
int CIEProxy::StringMatch(const string & format, const string & str)  
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

#endif
