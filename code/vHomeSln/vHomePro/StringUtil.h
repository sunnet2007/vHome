#pragma once

#include <windows.h>
#include <string>
using namespace std;

//*tchar��TCHAR����ָ�룬*_char��char����ָ��   
void TcharToChar (const TCHAR * tchar, char * _char)  ;

//charתTCHAR   
void CharToTchar (const char * _char, TCHAR * tchar);


string StringFromLPCTSTR(LPCTSTR str) ;
std::wstring s2ws(const std::string& s);

string LoadMyImage(const string& strDest);