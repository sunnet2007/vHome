#include "StdAfx.h"
#include "CommonTools.h"
#include <Windows.h>
#include <sstream>
using namespace std;


CCommonTools::CCommonTools(void)
{
}


CCommonTools::~CCommonTools(void)
{
}
bool  CCommonTools::GetModulePath( CBaseString &strModule )
{
	TCHAR szModule[256] = {0};
	GetModuleFileName( NULL,szModule,MAX_PATH);
	strModule = szModule;
	int pos = strModule.RFind( _T('\\') );
	strModule = strModule.Left( pos +1 );
	return true;
}
bool CCommonTools::UnicodeToUTF8( const wchar_t* pUnicode, int len,string &strUtf8 )  
{  
	// convert an widechar string to utf8  
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, pUnicode, len, NULL, 0, NULL, NULL);  
	if (utf8Len <= 0)  
	{  
		return ""; 
	}  
	char *pUtf8 = new char[utf8Len+1];
	if ( !pUtf8 )
	{
		return false;
	}
	memset( pUtf8,0,utf8Len+1);

	int nRtn = WideCharToMultiByte(CP_UTF8, 0,pUnicode, len, pUtf8, utf8Len, NULL, NULL);  

	if (nRtn != utf8Len)  
	{  
		delete []pUtf8;
		pUtf8 = NULL; 
		return false;
	}  

	strUtf8.append(pUtf8,utf8Len);
	delete []pUtf8;
	pUtf8 = NULL; 

	return true;  
} 


bool CCommonTools::UTF8ToUnicode(  const char* pu8, int utf8Len,wstring &strUnicode)  
{  
	// convert an UTF8 string to widechar   
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  

	wchar_t *pUnicode = new wchar_t[nLen + 1];
	if ( !pUnicode )
	{
		return false;
	}
	memset( pUnicode,0,nLen+1);

	int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, pUnicode, nLen);  

	if(nRtn != nLen)  
	{  
		return false;
	}  

	strUnicode.append(pUnicode,nLen);
	delete pUnicode;

	return true;  
}  

int CCommonTools::string_split(const string & str, vector<string> & arr, char delim)
{
	//arr.clear();

	stringstream sstr(str);
	string s;
	while(getline(sstr, s, delim))
	{
		arr.push_back(s);
	}

	return arr.size();
}

