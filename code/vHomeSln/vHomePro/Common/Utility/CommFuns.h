#ifndef _COMM_FUN_H_20120316
#define _COMM_FUN_H_20120316

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

#if (defined(_WIN32) || defined(_WIN64))

#include <windows.h>
#include <time.h>

#define snprintf   _snprintf
#define vsnprintf  _vsnprintf
#define strcasecmp stricmp
#define strcasestr stristr

#define int64 __int64

#pragma warning(disable : 4786)
#pragma comment(lib,"ws2_32.lib")

#else

#define int64 long long

#endif

string itoa64(int64 i);

int64 atoi64(string s);

int64 Hex2i64(string s);

int IsHex(string s);

string EscapeStr(const string & str);

string EscapeStr(int64 i);

string SqliteEscapeStr(const string & str);

string SqliteEscapeStr(int64 i);

#define ESC(str) EscapeStr(str).c_str()

#define SqliteEsc(str) SqliteEscapeStr(str).c_str()

int ParseUrl(string url, string & host, int & port, string & path);

int ParseUrl(string url, string & host, int & port);

int ParseHost(string host, string & ip);

int CvtAbsUrl(string url1, string path2, string & url2);

int TrimPath(string & path);

int TrimPathForWin(string & path); //for windows

bool IsDirWritable(string dir);

std::string UrlEncode(const std::string& szToEncode);
std::string UrlDecode(const std::string& szToDecode);

string EncodePath(const string & arg);

string DecodePath(const string & arg);

string EncodeUrl(const string & arg);

string EncodeUrlVal(const string & arg);

int SplitUrlArgs(const string & args, map<string, string> & mapArgs);

int GetUrlArg(const string & args, const string & arg, string & val);

int GetUrlArg(const string & args, const string & arg, int & val);

int GBKToUTF8(unsigned char * lpGBKStr,unsigned char * lpUTF8Str,int nUTF8StrLen);

int UTF8ToGBK(unsigned char * lpUTF8Str,unsigned char * lpGBKStr,int nGBKStrLen);

string UTF8ToGBK(const string & utf8Str);

string GBKToUTF8(const string & gbkStr);

bool IsTextUTF8(const char * str, int length = 0);

int MkDir(const string & dir);

int ReadFile(const string & fn, string & content);

int ReadFile(const string & fn, char ** content, unsigned long * len);

int WriteFile(const string & fn, string & content);

int DeleteFile(const string & fn);

int IsFileExist(const string & fn);

int GetFileExt(const string & fn, string & ext);

string GetMime(string ext);

string GetDateTime();

string ConvertDate(const string & date);

string ChangeSecToDateTime(time_t sec);

time_t ChangeDateTimeToSec(const string & dateTime);

#if (defined(_WIN32) || defined(_WIN64))

int CompareGMT(string t1, string t2);

int GetFileLastModifiedTime(string file, string & gmt);

bool ConvertFileTimeToSystemTime(FILETIME ft, SYSTEMTIME & st);

bool ConvertSystemTimeToFileTime(SYSTEMTIME st, FILETIME ft);

bool ConvertGmtToFileTime(string gmt, FILETIME & ft);

bool ConvertGmtToSystemTime(string gmt, SYSTEMTIME & st);

bool ConvertGmtToTime(string gmt, time_t & t);

int SetFileWriteTime(string fn, FILETIME * lpLastWriteTime);

#endif

int64 GetMilSec();

string GenGuid();

bool IsHexStr(string sHexStr);

bool IsGuidStr(string sGuidStr);

int CompareVersion(const string & v1, const string & v2);

int ReadUTF8CharsByCount(const char * str, const char * start, char * chs, int count);

int ReadUTF8CharsByCountReverse(const char * str, const char * start, char * chs, int count);

/*
#define AssignFieldInt(name, value) \
	if(value != NULL)\
	{\
		INT64 tmp;\
		sscanf(string(value).c_str(), "%I64d", &tmp);\
		name = tmp;\
	}

#define AssignFieldStr(name, value) \
	if(value != NULL)\
	{\
		name = value;\
	}\
	else\
	{\
		name = "";\
	}
*/

int XMLExtract(const string & xml, const string & tag, string & val);

int XMLExtract(const string & xml, const string & tag, string & val, int & pos);

int TextExtract(const string & text, const string & tag1, const string & tag2, string & val);

int TextExtract(const string & text, const string & tag1, const string & tag2, string & val, int & pos);

int ExtractLink(const string & src, map<string, string> & linkSet);

#define AssignFieldInt(name, value) \
	{\
		int64 tmp;\
		sscanf(string(value).c_str(), "%I64d", &tmp);\
		name = tmp;\
	}

#define AssignFieldStr(name, value) \
	{\
		name = value;\
	}

#endif
