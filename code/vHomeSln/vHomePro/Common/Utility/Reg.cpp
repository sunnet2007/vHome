
#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include <time.h>

using namespace std;

int GetProxyServer(string & host, int & port)
{
	HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut;
	if( ERROR_SUCCESS != RegOpenKeyExA(hKeyIn, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_CREATE_LINK | KEY_WRITE | KEY_READ | KEY_NOTIFY, &hKeyOut))
	{
		return 0;
	}
	
	DWORD proxyEnable = 0;
	ULONG regSize = sizeof(DWORD);
	if(ERROR_SUCCESS != RegQueryValueExA(hKeyOut, "ProxyEnable", NULL, NULL, (BYTE *)&proxyEnable, &regSize))
	{
		return -1;
	}

	if(!proxyEnable)
	{
		return -1;
	}

	char proxyServer[100] = { 0 };
	regSize = sizeof(proxyServer);
	if(ERROR_SUCCESS != RegQueryValueExA(hKeyOut, "ProxyServer", NULL, NULL, (BYTE *)&proxyServer, &regSize))
	{
		return -1;
	}

	string strProxyServer = proxyServer;
	if(strProxyServer.find("://") != string::npos)
	{
		strProxyServer = strProxyServer.substr(strProxyServer.find("://") + 3);
	}

	if(strProxyServer.find(":") != string::npos)
	{
		host = strProxyServer.substr(0, strProxyServer.find(":"));
		port = atoi(strProxyServer.substr(strProxyServer.find(":") + 1).c_str());
	}
	else
	{
		host = strProxyServer;
		port = 80;
	}

	return 0;
}

#if 0
int GetProxyServer(string & host, int & port)
{
	HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut;
	if( ERROR_SUCCESS != RegOpenKeyEx(hKeyIn, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_CREATE_LINK | KEY_WRITE | KEY_READ | KEY_NOTIFY, &hKeyOut))
	{
		LOG.Log("打开注册表失败\n");
		return -1;
	}
	LOG.Log("打开注册表成功\n");
	
	DWORD proxyEnable = 0;
	ULONG regSize = sizeof(DWORD);
	if(ERROR_SUCCESS != RegQueryValueEx(hKeyOut, "ProxyEnable", NULL, NULL, (BYTE *)&proxyEnable, &regSize))
	{
		LOG.Log("读取IE代理设置标志失败\n");
		return -1;
	}
	LOG.Log("读取IE代理设置标志成功\n");

	if(!proxyEnable)
	{
		LOG.Log("IE代理未启用\n");
		return -1;
	}
	LOG.Log("IE代理已启用\n");

	char proxyServer[100] = { 0 };
	regSize = sizeof(proxyServer);
	if(ERROR_SUCCESS != RegQueryValueEx(hKeyOut, "ProxyServer", NULL, NULL, (BYTE *)&proxyServer, &regSize))
	{
		LOG.Log("读取IE代理字符串失败\n");
		return -1;
	}
	LOG.Log("读取IE代理字符串：%s\n", proxyServer);

	string strProxyServer = proxyServer;
	if(strProxyServer.find("://") != string::npos)
	{
		strProxyServer = strProxyServer.substr(strProxyServer.find("://") + 3);
	}

	if(strProxyServer.find(":") != string::npos)
	{
		host = strProxyServer.substr(0, strProxyServer.find(":"));
		port = atoi(strProxyServer.substr(strProxyServer.find(":") + 1).c_str());
	}
	else
	{
		host = strProxyServer;
		port = 80;
	}

	LOG.Log("主机：%s 端口：%d\n", host.c_str(), port);

	return 0;
}
#endif


int GetRegValue_S(struct HKEY__ * ReRootKey, string ReSubKey, string ReValueName, string & ReGetContent_S)
{
	ReGetContent_S = "";

	HKEY hKey;
	if(RegOpenKeyExA(ReRootKey, ReSubKey.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		BYTE szData[1024] = {0};
		DWORD dwSize = sizeof(szData);
		if(RegQueryValueExA(hKey, ReValueName.c_str(), NULL, &dwType, szData, &dwSize) == ERROR_SUCCESS)
		{
			char sztmp[1024+1] = {0};
			memcpy(sztmp,szData,dwSize);
			ReGetContent_S = sztmp;
			RegCloseKey(hKey);
			return 0;
		}
		else
		{
			RegCloseKey(hKey);
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int SetRegValue_S(struct HKEY__ * ReRootKey, string ReSubKey, string ReValueName, string ReSetContent_S)
{
	HKEY hKey;
	if(RegCreateKeyA(ReRootKey, ReSubKey.c_str(), &hKey) != ERROR_SUCCESS)
	{
		return -1;
	}

	if(RegOpenKeyExA(ReRootKey, ReSubKey.c_str(), 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		if(RegSetValueExA(hKey, ReValueName.c_str(), NULL, REG_SZ, (const BYTE *)ReSetContent_S.c_str(), strlen(ReSetContent_S.c_str())) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return 0;
		}
		else
		{
			RegCloseKey(hKey);
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int GetRegValue_D(struct HKEY__ * ReRootKey, string ReSubKey, string ReValueName, DWORD & ReGetContent_D)
{
	ReGetContent_D = 0x00;

	HKEY hKey;
	if(RegOpenKeyExA(ReRootKey, ReSubKey.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_DWORD;
		BYTE szData[1024] = {0};
		DWORD dwSize = sizeof(szData);
		if(RegQueryValueExA(hKey, ReValueName.c_str(), NULL, &dwType, szData, &dwSize) == ERROR_SUCCESS)
		{
			memcpy(&ReGetContent_D ,szData, sizeof(DWORD));
			RegCloseKey(hKey);
			return 0;
		}
		else
		{
			RegCloseKey(hKey);
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int SetRegValue_D(struct HKEY__ * ReRootKey, string ReSubKey, string ReValueName, DWORD ReSetContent_D)
{
	HKEY hKey;
	if(RegCreateKeyA(ReRootKey, ReSubKey.c_str(), &hKey) != ERROR_SUCCESS)
	{
		return -1;
	}

	if(RegOpenKeyExA(ReRootKey, ReSubKey.c_str(), 0, KEY_WRITE, &hKey) == ERROR_SUCCESS)
	{
		if(RegSetValueExA(hKey, ReValueName.c_str(), NULL, REG_DWORD, (const BYTE *)&ReSetContent_D, sizeof(ReSetContent_D)) == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return 0;
		}
		else
		{
			RegCloseKey(hKey);
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

int DeleteRegSubKey(HKEY hKey, string subKey)
{
 	LONG lResult;
	HKEY hSubKey;
	DWORD   dwIndex, cbName; 
	char   szSubKey[512]; 
	FILETIME   ft; 
	lResult   =   RegOpenKeyExA(hKey,   subKey.c_str(),   0,   KEY_WOW64_64KEY | KEY_ALL_ACCESS,&hSubKey); 
	//lResult   =   RegOpenKeyEx(hKey,   lpSubKey,   0,   KEY_WOW64_64KEY | KEY_READ, &hSubKey); 
	HKEY hKeyWabPath = 0;
	/*
	//lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,  "SYSTEM\\ControlSet001\\services\\SmartBoxServer", 0, KEY_READ, &hKeyWabPath);
	if(ERROR_SUCCESS != lResult)
	{
		//LOG.Log("RegOpenKeyEx FAILE!!!");
		return false;
	}
	*/

	if (lResult != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		return -1;
	}
	dwIndex = 0;
	cbName = sizeof(szSubKey)/sizeof(szSubKey[0]);
	while (ERROR_SUCCESS == (lResult = RegEnumKeyExA(hSubKey, dwIndex, szSubKey, &cbName, NULL, NULL, NULL, &ft)))
	{
		DeleteRegSubKey(hSubKey, szSubKey);
	}
	RegCloseKey(hSubKey);
	lResult = RegDeleteKeyA(hKey, subKey.c_str());
	return 0;
}


