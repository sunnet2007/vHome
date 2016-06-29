#if (defined(_WIN32) || defined(_WIN64))

#ifndef _REG_H_20121210
#define _REG_H_20121210

#include <iostream>
#include <string>
using namespace std;

int GetProxyServer(string & host, int & port);

int GetRegValue_S(struct HKEY__ * ReRootKey, string ReSubKey, string ReValueName, string & ReGetContent_S);

int SetRegValue_S(struct HKEY__ * ReRootKey, string ReSubKey, string ReValueName, string ReSetContent_S);

int GetRegValue_D(struct HKEY__ * ReRootKey, string ReSubKey, string ReValueName, DWORD & ReGetContent_D);

int SetRegValue_D(struct HKEY__ * ReRootKey, string ReSubKey, string ReValueName, DWORD ReSetContent_D);

int DeleteRegSubKey(HKEY hKey, string subKey);

#endif

#endif
