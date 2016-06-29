#pragma once
#include <string>
#include <vector>
#include "base_string.h"

class  CCommonTools
{
public:
	CCommonTools(void);
	~CCommonTools(void);

	static bool   GetModulePath( CBaseString &strModule );
	static bool	  UnicodeToUTF8(  const wchar_t* pUnicode, int len,std::string &strUtf8 );  
	static bool   UTF8ToUnicode(  const char* pu8, int utf8Len,std::wstring &strUnicode );
	static int    string_split(   const std::string & str, std::vector<std::string> & arr, char delim);
};

