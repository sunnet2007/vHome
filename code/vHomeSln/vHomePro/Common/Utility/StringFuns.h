#ifndef _STRING_FUNS_H
#define _STRING_FUNS_H

/*
auth: liaoyan
last update: 2008-01-14 14:48
*/

#if (defined(_WIN32) || defined(_WIN64))

#include <windows.h>

#define snprintf   _snprintf
#define vsnprintf  _vsnprintf
#define strcasecmp stricmp
#define strncasecmp strnicmp
#define strcasestr stristr

#define int64 __int64

#pragma warning(disable : 4786)

#else

#define int64 long long

#endif

#include <iostream>
#include <sstream>
#include <vector>
#include <stdarg.h>
#include <algorithm>
using namespace std;

int StringReplace(string & str, const string & old_str, const string & new_str);

void StringTrim(string & text);

int StringSplit(string str, vector < string > & arr, char delim = '\n');

int StringSplit(string str, vector<string> & arr, string delim = "\n");

string StringPrintf(const char * format, ...);

int str_hex(const void * org, int org_len, void * dst);

int str_unhex(const void * org, int org_len, void * dst);

int StringUpper(string & s);

int StringLower(string & s);

string::size_type StringFindNoCase(string str, string substr, string::size_type start_pos = 0);

int StringWildcardMatch(const string & format, const string & str);

void WCharToChar(const wchar_t * wchar, char * chr, int length);

string ExtractHtmlText(const string & src, int startPos = 0);

#endif
