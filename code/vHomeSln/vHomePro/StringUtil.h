#pragma once

#include <windows.h>
#include <string>
using namespace std;

//*tchar是TCHAR类型指针，*_char是char类型指针   
void TcharToChar (const TCHAR * tchar, char * _char)  ;

//char转TCHAR   
void CharToTchar (const char * _char, TCHAR * tchar);


string StringFromLPCTSTR(LPCTSTR str) ;
std::wstring s2ws(const std::string& s);

string LoadMyImage(const string& strDest);