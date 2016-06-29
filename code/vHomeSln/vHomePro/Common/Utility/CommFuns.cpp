//#include <imagehlp.h>  
//#pragma comment(lib,"imagehlp.lib") 

#if (defined(_WIN32) || defined(_WIN64))

#include <windows.h>
#include <time.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define int64 long long
#define stricmp strcasecmp
#endif

#include <fstream>
#include "CommFuns.h"
#include "StringFuns.h"

string itoa64(int64 i) {
    char tmp[100];
#if (defined(_WIN32) || defined(_WIN64))
    sprintf(tmp, "%I64d", i);
#else
    sprintf(tmp, "%lld", i);
#endif

    return string(tmp);
}

int64 atoi64(string s) {
    int j = 0;
    while(j < s.size()) {
        if(s[j] != ' ' && s[j] != '\t')
            break;
        j++;
    }
    if(j == s.size())
        return 0;
    if(!((s[j] >= '0' && s[j] <= '9') || s[j] == '+' || s[j] == '-'))
        return 0;

    int64 i;
#if (defined(_WIN32) || defined(_WIN64))
    sscanf(s.c_str(), "%I64d", &i);
#else
    sscanf(s.c_str(), "%lld", &i);
#endif

    return i;
}

int IsHex(string s) {
    int i;
    for(i = 0; i < s.size(); i++) {
        if(!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')))
            return 0;
    }

    return 1;
}

string EscapeStr(const string & str) {
	string s = str;

	string old_str, new_str;
	string::size_type pos;
	string::size_type oldlen;
	string::size_type newlen;

	old_str = "'";
	new_str = "''";
	pos = 0;
	oldlen = old_str.size();
	newlen = new_str.size();
	while((pos=s.find(old_str, pos)) != string::npos) {
		s.replace(pos, oldlen, new_str);
		pos += newlen;
	}
/*
	old_str = "\\";
	new_str = "\\\\";
	pos = 0;
	oldlen = old_str.size();
	newlen = new_str.size();
	while((pos=s.find(old_str, pos)) != string::npos) {
		s.replace(pos, oldlen, new_str);
		pos += newlen;
	}
	*/
	return s;
}

string EscapeStr(int64 i) {
	return itoa64(i);
}


string SqliteEscapeStr(const string & str) {
	string s = str;

	string old_str, new_str;
	string::size_type pos;
	string::size_type oldlen;
	string::size_type newlen;

	old_str = "'";
	new_str = "''";
	pos = 0;
	oldlen = old_str.size();
	newlen = new_str.size();
	while((pos=s.find(old_str, pos)) != string::npos) {
		s.replace(pos, oldlen, new_str);
		pos += newlen;
	}

	return s;
}

string SqliteEscapeStr(int64 i) {
	return itoa64(i);
}

int ParseUrl(string url, string & host, int & port, string & path) {
	if(url.size() == 0)
		return -1;

	string::size_type p0, p1, p2;

	/*
	if(stricmp(url.substr(0,7).c_str(), "http://") != 0) {
		return -1;
	}
	*/

	if((p0 = url.find("://")) == string::npos) {
		p0 = 0;
	}
	else {
		p0 += 3;
	}

	p1 = url.find(":", p0);
	p2 = url.find("/", p0);

	if(p1 != string::npos && p2 != string::npos)
	{
		if(p1 > p2)
			p1 = string::npos;
	}

	if(p1 != string::npos && p2 != string::npos) {
		host = url.substr(p0, min(p1, p2)-p0);
		port = atoi(url.substr(p1+1, p2-p1-1).c_str());
		path = url.substr(p2);
	}
	else if(p1 != string::npos) {
		host = url.substr(p0, p1-p0);
		port = atoi(url.substr(p1+1).c_str());
		path = "/";
	}
	else if(p2 != string::npos) {
		host = url.substr(p0, p2-p0);
		port = 80;
		path = url.substr(p2);
	}
	else {
		host = url.substr(p0);
		port = 80;
		path = "/";
	}

	if(port <= 0)
		port = 80;

	if(host.size() == 0)
		return -1;

	return 0;
}

int ParseUrl(string url, string & host, int & port)
{
	string path;
	return ParseUrl(url, host, port, path);
}

int ParseHost(string host, string & ip)
{
	ip = "";
	if(ntohl(inet_addr(host.c_str())) != 0xffffffff
		&& ntohl(inet_addr(host.c_str())) > ntohl(inet_addr("1.0.0.0")))
	{
		ip = host;
	}
	else
	{
		struct hostent * phe;
		phe = gethostbyname(host.c_str()); 
		if (phe == NULL)
		{
			return -1;  
		} 
		else
		{
			struct in_addr addr;
			addr.s_addr = *(unsigned long *) phe->h_addr_list[0]; 
			ip = inet_ntoa(addr);
		}
	}

	return 0;
}


//把按照url1，把path2转成绝对路径接后存入url2
//例如，
//url1 = http://www.music.com/mp3/wakin/ab.mp3
//path2 = ../andy/cd.mp3
//则
//url2 = http://www.music.com/mp3/andy/cd.mp3

int CvtAbsUrl(string url1, string path2, string & url2)
{
	url2 = "";

	//如果path2已经是绝对路径，就直接拷贝
	/*if(strncasecmp(path2, "http://", 7) == 0
		|| strncasecmp(path2, "https://", 8) == 0
		|| strncasecmp(path2, "ftp://", 6) == 0
		|| strncasecmp(path2, "thunder://", 10) == 0
		|| strncasecmp(path2, "mms://", 6) == 0
		|| strncasecmp(path2, "rtsp://", 7) == 0
		|| strncasecmp(path2, "javascript:", 11) == 0
		||strncasecmp(path2, "mailto:", 7) == 0
		||strncasecmp(path2, "tencent://", 10) == 0)
	{
		strcpy(url2, path2);
		return 0;
	}*/
	
	int i = 0;
	while(i < path2.size() && ((path2[i] >= 'a' && path2[i] <= 'z') || (path2[i] >= 'A' && path2[i] <= 'Z')))
	{
		i++;
	}
	if(i > 0 && i < path2.size() && path2[i] == ':')
	{
		//strcpy(url2, path2);
		url2 = path2;
		return 0;
	}

	//解析url1
	char host[1024];
	char path1[2048];
	if(strncasecmp(url1.c_str(), "http://", 7) == 0)
	{
		i = 7;
	}
	else if(strncasecmp(url1.c_str(), "ftp://", 6) == 0)
	{
		i = 6;
	}
	else
	{
		//strcpy(url2, "");
		return -1;
	}

	int j = 0;
	while(i < url1.size() && j < sizeof(host)-1 && url1[i] != '/' && url1[i] != '\0')
	{
		host[j] = url1[i];
		i++;
		j++;
	}
	host[j] = '\0';

	j = 0;
	while(i < url1.size() && url1[i] != '\0')
	{
		path1[j] = url1[i];
		i++;
		j++;
	}
	path1[j] = '\0';

	if(path1[0] == '\0')
	{
		strcpy(path1, "/");
	}

	const char * p1 = path1 + strlen(path1) - 1;
	const char * p2 = path2.c_str();

	while(*p2 == ' ' || *p2 == '\t')
	{
		p2++;
	}

	if(p2[0] == '/')
	{
		//sprintf(url2, "http://%s%s", host, p2);
		url2 = string("http://") + host + p2;
		return 0;
	}
	
	while(p1 >= path1 && *p1 != '/')
	{
		p1--;
	}
	if(*p1 != '/') //error
	{
		//strcpy(url2, "");
		return -1;
	}
	p1--;

	while(strncmp(p2, "../", 3) == 0)
	{
		p2 += 3;
		while(p1 >= path1 && *p1 != '/')
		{
			p1--;
		}
		if(*p1 != '/') //error
		{
			//return -1;
			//some times, we should ignore this error.
			break;
		}
		
		p1--;
	}

	while(strncmp(p2, "./", 2) == 0)
	{
		p2 += 2;
	}

	char tmppath[500];

	strncpy(tmppath, path1, (int)(p1-path1+1));
	tmppath[(int)(p1-path1+1)] = '/';
	tmppath[(int)(p1-path1+2)] = '\0';
	//strcat(tmppath, p2);
	//sprintf(url2, "http://%s%s%s", host, tmppath, p2);
	url2 = string("http://") + host + tmppath + p2;
		
	
	return 0;
} 

int TrimPath(string & path) {
	if(path.size() <= 1)
		return 0;

	int i;
	for(i = 0; i < path.size(); i++) {
		if(path[i] == '\\')
			path[i] = '/';
	}

	int head = 0, tail = 0;
	if(path[0] == '/') {
		head = 1;
	}
	if(path[path.size()-1] == '/') {
		tail = 1;
	}

	path += "/";

	vector<string> name_list;
	string::size_type last = -1, cur = -1;
	while((cur = path.find("/", last+1)) != string::npos) {
		string name = path.substr(last+1, cur-last-1);
		if(name.size() > 0) {
			name_list.push_back(name);
		}

		last = cur;
	}

	for(i = 0; i < name_list.size(); i++) {
		if(name_list[i] == ".") {
			name_list[i] = "";
		}
		else if(name_list[i] == "..") {
			name_list[i] = "";
			int j;
			for(j = i-1; j >= 0; j--) {
				if(name_list[j].size() > 0) {
					name_list[j] = "";
					break;
				}
			}
		}
	}

	if(head) {
		path = "/";
	}
	else {
		path = "";
	}

	for(i = 0; i < name_list.size(); i++) {
		if(name_list[i].size() > 0) {
			path += name_list[i];
			if(tail) {
				path += "/";
			}
			else if(i < name_list.size()-1) {
				path += "/";
			}
		}
	}

	return 0;
}

//for windows
int TrimPathForWin(string & path) {
	if(path.size() <= 1)
		return 0;

	int i;
	for(i = 0; i < path.size(); i++) {
		if(path[i] == '/')
			path[i] = '\\';
	}

	int head = 0, tail = 0;
	if(path[0] == '\\') {
		head = 1;
	}
	if(path[path.size()-1] == '\\') {
		tail = 1;
	}

	path += "\\";

	vector<string> name_list;
	string::size_type last = -1, cur = -1;
	while((cur = path.find("\\", last+1)) != string::npos) {
		string name = path.substr(last+1, cur-last-1);
		if(name.size() > 0) {
			name_list.push_back(name);
		}

		last = cur;
	}

	for(i = 0; i < name_list.size(); i++) {
		if(name_list[i] == ".") {
			name_list[i] = "";
		}
		else if(name_list[i] == "..") {
			name_list[i] = "";
			int j;
			for(j = i-1; j >= 0; j--) {
				if(name_list[j].size() > 0) {
					name_list[j] = "";
					break;
				}
			}
		}
	}

	if(head) {
		path = "\\";
	}
	else {
		path = "";
	}

	for(i = 0; i < name_list.size(); i++) {
		if(name_list[i].size() > 0) {
			path += name_list[i];
			if(tail) {
				path += "\\";
			}
			else if(i < name_list.size()-1) {
				path += "\\";
			}
		}
	}

	return 0;
}

bool IsDirWritable(string dir)
{
	char tmpFn[1024] = { 0 };
	snprintf(tmpFn, sizeof(tmpFn), "%s\\tmpfn.%u.%u.test", dir.c_str(), (unsigned int)time(NULL), (unsigned int)rand());

	FILE * fp = fopen(tmpFn, "wb");
	if(fp == NULL)
		return false;

	fclose(fp);
	unlink(tmpFn);

	return true;
}

inline unsigned char toHex(const unsigned char &x)
{
    return x > 9 ? x + 55: x + 48;
}

unsigned char FromHex(unsigned char x) 
{ 
    unsigned char y = 0;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else 
    {
        //assert(0);
    }
    return y;
}

std::string UrlEncode(const std::string& szToEncode)
{
    std::string src = szToEncode;
    char hex[] = "0123456789ABCDEF";
    string dst;

    for (size_t i = 0; i < src.size(); ++i)
    {
        unsigned char cc = src[i];

        if( isalnum( cc ) ||
            (cc == '-') ||  
            (cc == '_') ||   
            (cc == '.') ||   
            (cc == '~'))
        {
            dst += cc;
        }
        else if ( isspace( cc ) )
        {
            dst += "%20";
        }
        else
        {
            unsigned char c = static_cast<unsigned char>(src[i]);
            dst += '%';
            dst += toHex( c >> 4 );
            dst += toHex( c % 16);
        }
    }

    return dst;
}


std::string UrlDecode(const std::string& szToDecode)
{
    std::string result;
    int hex = 0;
    for (size_t i = 0; i < szToDecode.length(); ++i)
    {
        switch (szToDecode[i])
        {
        case '+':
            result += ' ';
            break;
        case '%':
#if 1
            {

                if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))  
                {
                    std::string hexStr = szToDecode.substr(i + 1, 2);
                    unsigned char high = FromHex((unsigned char)hexStr[0]);
                    unsigned char low = FromHex((unsigned char)hexStr[1]);
                    hex = high*16 + low;

                    result += char(hex);
                    i += 2;
                }
                else 
                {  
                    result += '%';  
                }  
                break;  
            }
#else
            {
                if (isxdigit(szToDecode[i + 1]) && isxdigit(szToDecode[i + 2]))  
                {
                    std::string hexStr = szToDecode.substr(i + 1, 2);  
                    unsigned char high = FromHex((unsigned char)hexStr[0]);
                    unsigned char low = FromHex((unsigned char)hexStr[1]);
                    hex = high * 16 + low;

                    //字母和数字[0-9a-zA-Z]、一些特殊符号[$-_.+!*'(),] 、以及某些保留字[$&+,/:;=?@]  
                    //可以不经过编码直接用于URL  
                    if (!((hex >= 48 && hex <= 57) || //0-9  
                        (hex >=97 && hex <= 122) ||   //a-z  
                        (hex >=65 && hex <= 90) ||    //A-Z  
                        //一些特殊符号及保留字[$-_.+!*'(),]  [$&+,/:;=?@]  
                        hex == 0x21 || hex == 0x24 || hex == 0x26 || hex == 0x27 || hex == 0x28 || hex == 0x29 
                        || hex == 0x2a || hex == 0x2b|| hex == 0x2c || hex == 0x2d || hex == 0x2e || hex == 0x2f 
                        || hex == 0x3A || hex == 0x3B|| hex == 0x3D || hex == 0x3f || hex == 0x40 || hex == 0x5f 
                        ))  
                    {  
                        result += char(hex);
                        i += 2;  
                    }  
                    else
                    {
                        result += '%'; 
                    }
                }
                else 
                {  
                    result += '%';  
                }  
                break;  
            }
#endif
        default:
            result += szToDecode[i];
            break;
        }
    }
    return result;
}

string EncodePath(const string & arg)
{
	string ret;
	int i;
	int needEncode = 0;
	for(i = 0; i < arg.size(); i++)
	{
		unsigned char ch = (unsigned char)arg[i];
		if(ch == ' ' || ch == '\t' || ch > (unsigned char)127)
		{
			needEncode = 1;
			break;
		}
	}

	if(!needEncode)
		return arg;

	for(i = 0; i < arg.size(); i++)
	{
		unsigned char ch = (unsigned char)arg[i];
		if((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
			|| ch == '_' || ch == '-' || ch == '+' || ch == '.' || ch == '/' || ch == '?' || ch == '&' || ch == ':')
		{
			ret.append(1, ch);
		}
		else if(ch == '\\') {
			ret.append(1, '/');
		}
		else
		{
			char hex[10];
			sprintf(hex, "%%%02X", ch);
			ret += hex;
		}
	}

	return ret;
}

string DecodePath(const string & arg) {
	string ret;
	int i;
	for(i = 0; i < arg.size(); i++) {
		unsigned char ch;
		if(arg[i] == '%' && i < arg.size()-2 && isxdigit(arg[i + 1]) && isxdigit(arg[i + 2])) {
				char str[3];
				unsigned int n;
				str[0] = arg[i+1];
				str[1] = arg[i+2];
				str[2] = '\0';
				sscanf(str, "%x" , &n);
				ch = (unsigned char)n;
				
				i += 2;
		}
        else if (arg[i] == '+')
        {
            ch = ' ';
        }
		else {
			ch = arg[i];
		} 

		ret.append(1, ch);
	}

	return ret;
}

string EncodeUrl(const string & arg)
{
	int i;
	string ret;
	for(i = 0; i < arg.size(); i++)
	{
		unsigned char ch = (unsigned char)arg[i];
		if((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
			|| ch == '_' || ch == '-' || ch == '+' || ch == '.')
		{
			ret.append(1, ch);
		}
		else if(ch == '\\') {
			ret.append(1, '/');
		}
		else
		{
			char hex[10];
			sprintf(hex, "%%%02X", ch);
			ret += hex;
		}
	}

	return ret;
}

string EncodeUrlVal(const string & arg)
{
	string ret;
	int i;
	for(i = 0; i < arg.size(); i++)
	{
		unsigned char ch = (unsigned char)arg[i];
		if((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
			|| ch == '_' || ch == '-' || ch == '.' || ch == ',' || ch == ':' || ch == '|')
		{
			ret.append(1, ch);
		}
		/*
		else if(ch == ' ')
		{
			ret += "+";
		}
		*/
		else
		{
			char hex[10];
			sprintf(hex, "%%%02X", ch);
			ret += hex;
		}
	}

	return ret;
}

int SplitUrlArgs(const string & args, map<string, string> & mapArgs)
{
	mapArgs.size();

	vector<string> vecArgFragment;
	StringSplit(args, vecArgFragment, "&");
	int i;
	for(i = 0; i < vecArgFragment.size(); i++)
	{
		vector<string> vecArgVal;
		StringSplit(vecArgFragment[i], vecArgVal, "=");
		if(vecArgVal.size() == 2)
		{
			bool bExisted = false;
			map<string, string>::iterator it;
			for(it = mapArgs.begin(); it != mapArgs.end(); it++)
			{
				if(stricmp(vecArgVal[0].c_str(), it->first.c_str()) == 0)
					bExisted = true;
			}

			if(!bExisted)
				mapArgs.insert(map<string, string>::value_type(vecArgVal[0], vecArgVal[1]));
		}
	}

	return 0;
}

int GetUrlArg(const string & args, const string & arg, string & val)
{
	map<string, string> mapArgs;
	SplitUrlArgs(args, mapArgs);

	map<string, string>::iterator it;
	for(it = mapArgs.begin(); it != mapArgs.end(); it++)
	{
		if(stricmp(arg.c_str(), it->first.c_str()) == 0)
		{
			val = it->second;
			return 0;
		}
	}

	return -1;
}

int GetUrlArg(const string & args, const string & arg, int & val)
{
	string strVal;
	if(GetUrlArg(args, arg, strVal) == 0)
	{
		val = atoi(strVal.c_str());
		return 0;
	}
	else
	{
		return -1;
	}
}

bool IsTextUTF8(const char* str, int length)
{
	if(length <= 0)
		length = strlen(str);
	
	int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr;
	bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
	for(int i = 0; i < length; i++)
	{
		chr = *(str + i);
		if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
		{
			bAllAscii= false;
		}

		if(nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr >= 0x80)
			{
				if(chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if(chr >= 0xF8)
					nBytes = 5;
				else if(chr >= 0xF0)
					nBytes = 4;
				else if(chr >= 0xE0)
					nBytes = 3;
				else if(chr >= 0xC0)
					nBytes = 2;
				else
				{
					return false;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return false;
			}
			nBytes--;
		}
	}

	if( nBytes > 0 ) //违返规则
	{
		return false;
	}

	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
	{
		return false;
	}

	return true;
}

#if (defined(_WIN32)|| defined(_WIN64))

wstring MultiByteToWideChar(const char* pszMultiByte, int iCodePage)
{
	wchar_t* pwszWideChar = NULL;
	try
	{
		int iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, NULL, 0);
		if (0 == iWideCharSize)
		{
			throw -1;
		}

		pwszWideChar = new wchar_t[iWideCharSize+1];
		memset(pwszWideChar, 0, (iWideCharSize+1)*sizeof(wchar_t));

		iWideCharSize = ::MultiByteToWideChar(iCodePage, 0, pszMultiByte, -1, pwszWideChar, iWideCharSize);
		if (0 == iWideCharSize)
		{
			throw -1;
		}

		wstring wstrWideChar = pwszWideChar;

		delete[] pwszWideChar;
		pwszWideChar = NULL;

		return wstrWideChar;
	}
	catch (...)
	{
		if (NULL != pwszWideChar)
		{
			delete[] pwszWideChar;
			pwszWideChar = NULL;
		}

		return L"";
	}
}

string WideCharToMultiByte(const wchar_t* pwszWideChar, int iCodePage) 
{
	char* pszMultiByte = NULL;
	try
	{
		int iMultiByteSize = ::WideCharToMultiByte(iCodePage, 0, pwszWideChar, -1, NULL, 0, NULL, NULL);
		if (0 == iMultiByteSize)
		{
			throw -1;
		}

		pszMultiByte = new char[iMultiByteSize+1];
		memset(pszMultiByte, 0, (iMultiByteSize+1)*sizeof(char));

		iMultiByteSize = ::WideCharToMultiByte(iCodePage, 0, pwszWideChar, -1, pszMultiByte, iMultiByteSize, NULL, NULL);
		if (0 == iMultiByteSize)
		{
			throw -1;
		}

		string strMultiByte = pszMultiByte;

		delete[] pszMultiByte;
		pszMultiByte = NULL;

		return strMultiByte;
	}
	catch (...)
	{
		if (NULL != pszMultiByte)
		{
			delete[] pszMultiByte;
			pszMultiByte = NULL;
		}

		return "";
	}
}

bool ConvUtf8ToAnsi(const string& strSource, string& strChAnsi)  
{   
	strChAnsi = "";  
	if (strSource.length() == 0 )  return 0;  	

	if ( !IsTextUTF8(strSource.c_str(), strSource.length()) )
	{
		strChAnsi = strSource;
		return true;
	}

	wstring wstr = MultiByteToWideChar(strSource.c_str(), CP_UTF8);
	strChAnsi = WideCharToMultiByte(wstr.c_str(), CP_ACP);

	return strChAnsi.empty();
}

/*
string UTF8ToGBK(const string & utf8Str)
{
	string gbkStr;
	ConvUtf8ToAnsi(utf8Str, gbkStr);

	return gbkStr;
}
*/

#endif


int MkDir(const string & path) {
	if(path.size() <= 1)
		return -1;

	string tmpPath = path;

	int i;
	for(i = 0; i < tmpPath.size(); i++) {
		if(tmpPath[i] == '\\')
			tmpPath[i] = '/';
	}

	int head = 0, tail = 0;
	if(tmpPath[0] == '/') {
		head = 1;
	}
	if(tmpPath[tmpPath.size()-1] == '/') {
		tail = 1;
	}

	tmpPath += "/";

	vector<string> name_list;
	string::size_type last = -1, cur = -1;
	while((cur = tmpPath.find("/", last+1)) != string::npos) {
		string name = tmpPath.substr(last+1, cur-last-1);
		if(name.size() > 0) {
			name_list.push_back(name);
		}

		last = cur;
	}

	for(i = 0; i < name_list.size(); i++) {
		if(name_list[i] == ".") {
			name_list[i] = "";
		}
		else if(name_list[i] == "..") {
			name_list[i] = "";
			int j;
			for(j = i-1; j >= 0; j--) {
				if(name_list[j].size() > 0) {
					name_list[j] = "";
					break;
				}
			}
		}
	}

	if(head) {
		tmpPath = "/";
	}
	else {
		tmpPath = "";
	}

	for(i = 0; i < name_list.size(); i++) {
		if(name_list[i].size() > 0) {
			tmpPath += name_list[i];

			#if (defined(_WIN32) || defined(_WIN64))
			/*
			if(CreateDirectory(tmpPath.c_str(), NULL) == FALSE)
			{
				if(GetLastError() != 183)
					return -1;
			}
			*/
			CreateDirectoryA(tmpPath.c_str(), NULL);
			#else
			mkdir(tmpPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			#endif

			if(tail) {
				tmpPath += "/";
			}
			else if(i < name_list.size()-1) {
				tmpPath += "/";
			}
		}
	}

	return 0;
}



#if (defined(_WIN32) || defined(_WIN64))

#define GBK_CODEPAGE CP_ACP

// UTF8编码转换到GBK编码
int UTF8ToGBK(unsigned char * lpUTF8Str,unsigned char * lpGBKStr,int nGBKStrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;
	if(!lpUTF8Str) //如果UTF8字符串为NULL则出错退出
		return 0;
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,NULL,NULL); //获取转换到Unicode编码后所需要的字符空间长度
	lpUnicodeStr = new WCHAR[nRetLen + 1]; //为Unicode字符串空间
	nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,lpUnicodeStr,nRetLen); //转换到Unicode编码
	if(!nRetLen) //转换失败则出错退出
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}
	//nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,NULL,NULL,NULL,NULL); //获取转换到GBK编码后所需要的字符空间长度
    nRetLen = ::WideCharToMultiByte(GBK_CODEPAGE,0,lpUnicodeStr,-1,NULL,NULL,NULL,NULL); //获取转换到GBK编码后所需要的字符空间长度
	if(!lpGBKStr) //输出缓冲区为空则返回转换后需要的空间大小
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return nRetLen;
	}

	if(nGBKStrLen < nRetLen) //如果输出缓冲区长度不够则退出
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}

	//nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,(char *)lpGBKStr,nRetLen,NULL,NULL); //转换到GBK编码
    nRetLen = ::WideCharToMultiByte(GBK_CODEPAGE,0,lpUnicodeStr,-1,(char *)lpGBKStr,nRetLen,NULL,NULL); //转换到GBK编码
	if(lpUnicodeStr)
		delete []lpUnicodeStr;
	return nRetLen;
}

//GBK编码转换到UTF8编码
int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str,int nUTF8StrLen)
{
	wchar_t * lpUnicodeStr = NULL;
	int nRetLen = 0;
	if(!lpGBKStr) //如果GBK字符串为NULL则出错退出
		return 0;
	//nRetLen = ::MultiByteToWideChar(CP_ACP,0,(char *)lpGBKStr,-1,NULL,NULL); //获取转换到Unicode编码后所需要的字符空间长度
    nRetLen = ::MultiByteToWideChar(GBK_CODEPAGE,0,(char *)lpGBKStr,-1,NULL,NULL); //获取转换到Unicode编码后所需要的字符空间长度
	lpUnicodeStr = new WCHAR[nRetLen + 1]; //为Unicode字符串空间
	//nRetLen = ::MultiByteToWideChar(CP_ACP,0,(char *)lpGBKStr,-1,lpUnicodeStr,nRetLen); //转换到Unicode编码
    nRetLen = ::MultiByteToWideChar(GBK_CODEPAGE,0,(char *)lpGBKStr,-1,lpUnicodeStr,nRetLen); //转换到Unicode编码
	if(!nRetLen) //转换失败则出错退出
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}
	nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,NULL,0,NULL,NULL); //获取转换到UTF8编码后所需要的字符空间长度
	if(!lpUTF8Str) //输出缓冲区为空则返回转换后需要的空间大小
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return nRetLen;
	}
	if(nUTF8StrLen < nRetLen) //如果输出缓冲区长度不够则退出
	{
		if(lpUnicodeStr)
			delete []lpUnicodeStr;
		return 0;
	}
	nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,(char *)lpUTF8Str,nUTF8StrLen,NULL,NULL); //转换到UTF8编码
	if(lpUnicodeStr)
		delete []lpUnicodeStr;
	return nRetLen;
}

#else

/*
int UTF8ToGBK(unsigned char * lpUTF8Str,unsigned char * lpGBKStr,int nGBKStrLen)
{
	char* buf= NULL; 
	setlocale(LC_ALL, "zh_CN.UTF8"); 
	int nRetLen = wcstombs( NULL,(wchar_t*)lpUTF8Str, 0); 
	if(nRetLen<=0)  
	{  
		printf("Can not Transfer!!!\n");  
		return -1;  
	}
	buf = new char[nRetLen + 1]; 
	wcstombs(buf, (wchar_t*)lpUTF8Str, nRetLen); 
	buf[nRetLen] = '\0'; 
	if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//设置转换为unicode前的码,当前为gbk编码  
	{  
		free(buf);
		return -1;  
	}  
	int iGBkLen=mbstowcs(NULL,buf,0);//计算转换后的长度  
	if(iGBkLen<=0)  
	{  
		free(buf);
		return -1;  
	}     
	mbstowcs((wchar_t*)lpGBKStr,buf,iGBkLen);//将gbk转换为unicode 
	lpGBKStr[iGBkLen]=0;
	free(buf);
	return iGBkLen; 
}

int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str,int nUTF8StrLen)
{
	if(NULL== lpGBKStr)  
	{  
		return -1;  
	}  

	//首先先将gbk编码转换为unicode编码  
	if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//设置转换为unicode前的码,当前为gbk编码  
	{  
		return -1;  
	}  

	int unicodeLen=mbstowcs(NULL,(char*)lpGBKStr,0);//计算转换后的长度  
	if(unicodeLen<=0)  
	{  
		return -1;  
	}      
	wchar_t *unicodeStr=(wchar_t *)calloc(sizeof(wchar_t),unicodeLen+1);  
	mbstowcs(unicodeStr,(char*)lpGBKStr,strlen(((char*)lpGBKStr)));//将gbk转换为unicode  

	//将unicode编码转换为utf8编码  
	if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//设置unicode转换后的码,当前为utf8  
	{
		free(unicodeStr);
		return -1;  
	}  
	int utfLen=wcstombs(NULL,unicodeStr,0);//计算转换后的长度  
	if(utfLen<=0)  
	{
		free(unicodeStr);
		return -1;  
	}  
	else if(utfLen>=nUTF8StrLen)//判断空间是否足够  
	{
		free(unicodeStr);
		return -1;  
	}  
	wcstombs((char*)lpUTF8Str,unicodeStr,utfLen);  
	lpUTF8Str[utfLen]=0;//添加结束符  
	free(unicodeStr);  
	return utfLen;
}
*/

const char * ConvertCharset(char * cpEncodeFrom, char * cpEncodeTo, char * cpInput, char * cpOut, size_t * iOutLen)
{
        if(cpEncodeFrom == NULL || cpEncodeTo == NULL || cpInput == NULL || cpOut == NULL)
                return NULL;

        size_t iInputLen;

        iconv_t c_pt;
        if((c_pt = iconv_open(cpEncodeTo, cpEncodeFrom)) == (iconv_t)-1)
        {
                printf("iconv_open failed!\n");
                return NULL;
        }

        iInputLen = strlen(cpInput);
        if(iconv(c_pt, &cpInput, &iInputLen, &cpOut, iOutLen) == -1)
        {
                iconv_close(c_pt);
                return NULL;
        }

        iconv_close(c_pt);

        return cpOut;
}

int GBKToUTF8(unsigned char * lpGBKStr, unsigned char * lpUTF8Str, int nUTF8StrLen)
{
	int inlen = strlen((const char *)lpGBKStr);
	char * input = new char[inlen+1];
	//char * output = new char[inlen*3+1];
	//size_t outlen = inlen*3;

	memset(input, 0, inlen+1);
	memset(lpUTF8Str, 0, nUTF8StrLen);

	strcpy(input, (const char *)lpGBKStr);

	int ret = 0;
	size_t uUTF8Len = nUTF8StrLen;
	if(ConvertCharset("gbk", "utf-8", input, (char *)lpUTF8Str, &uUTF8Len) != NULL) {
		lpUTF8Str[nUTF8StrLen] = '\0';
		ret = nUTF8StrLen;
	}

	//strncpy(lpUTF8Str, output, nUTF8StrLen

	delete [] input;
	//delete [] output;

	return ret;
}

int UTF8ToGBK(unsigned char * lpUTF8Str, unsigned char * lpGBKStr, int nGBKStrLen)
{
	int inlen = strlen((const char *)lpUTF8Str);
	char * input = new char[inlen+1];
	//char * output = new char[inlen*3+1];
	//size_t outlen = inlen*3;

	memset(input, 0, inlen+1);
	memset(lpGBKStr, 0, nGBKStrLen);

	strcpy(input, (const char *)lpUTF8Str);

	int ret = 0;
	size_t nGBKlen = nGBKStrLen;
	if(ConvertCharset("utf-8", "gbk", input, (char *)lpGBKStr, &nGBKlen) != NULL) {
		lpGBKStr[nGBKStrLen] = '\0';
		ret = nGBKStrLen;
	}

	//strncpy(lpUTF8Str, output, nUTF8StrLen

	delete [] input;
	//delete [] output;

	return ret;
}


#endif

string UTF8ToGBK(const string & utf8Str)
{
	if(!IsTextUTF8(utf8Str.c_str(), utf8Str.size()))
	{
		return utf8Str;
	}

	int bufLen = utf8Str.size()*2+1;
	char * gbkStr = new char[bufLen];
	if(gbkStr == NULL)
		return "";

	memset(gbkStr, 0, bufLen);

	if(UTF8ToGBK((unsigned char *)utf8Str.c_str(), (unsigned char *)gbkStr, bufLen) == 0)
	{
		delete[] gbkStr;
		return utf8Str;
	}
	else
	{
		string gbk = gbkStr;
		delete[] gbkStr;
		return gbk;
	}
}

string GBKToUTF8(const string & gbkStr)
{
	if(IsTextUTF8(gbkStr.c_str(), gbkStr.size()))
	{
		return gbkStr;
	}

	int bufLen = gbkStr.size()*2+1;
	char * utf8Str = new char[bufLen];
	if(utf8Str == NULL)
		return "";

	memset(utf8Str, 0, bufLen);

	if(GBKToUTF8((unsigned char *)gbkStr.c_str(), (unsigned char *)utf8Str, bufLen) == 0)
	{
		delete[] utf8Str;
		return gbkStr;
	}
	else
	{
		string utf8 = utf8Str;
		delete[] utf8Str;
		return utf8;
	}
}

int XMLExtract(const string & xml, const string & tag, string & val, int & pos)
{
	val = "";

	string::size_type p1, p2;
	p1 = xml.find("<"+tag, pos);
	if(p1 == string::npos)
		return -1;

	p1 = xml.find(">", p1);
	if(p1 == string::npos)
		return -1;

	p1 += 1;
	p2 = xml.find("</"+tag+">", p1);
	if(p2 == string::npos)
		return -1;

	val = xml.substr(p1, p2-p1);
	pos = p1;

	if(val.substr(0, 9) == "<![CDATA[")
	{
		val = val.substr(9, val.size()-12);
	}
	else
	{
		StringReplace(val, "&lt;", "<");
		StringReplace(val, "&gt;", ">");
	}

	string::size_type utf8Pos = xml.find("encoding=\"utf-8\"");
	if(utf8Pos != string::npos && utf8Pos < 38)
	{
		val = UTF8ToGBK(val);
	}

	return 0;
}

int XMLExtract(const string & xml, const string & tag, string & val)
{
	int pos = 0;
	return XMLExtract(xml, tag, val, pos);
}

int TextExtract(const string & text, const string & tag1, const string & tag2, string & val, int & pos)
{
	val = "";

	string::size_type p1, p2;
	p1 = text.find(tag1, pos);
	if(p1 == string::npos)
		return -1;

	p1 += tag1.size();
	p2 = text.find(tag2, p1);
	if(p2 == string::npos)
		return -1;

	val = text.substr(p1, p2-p1);
	pos = p1;

	return 0;
}

int TextExtract(const string & text, const string & tag1, const string & tag2, string & val)
{
	int pos = 0;
	return TextExtract(text, tag1, tag2, val, pos);
}

int ReadFile(const string & fn, string & content)
{
	ifstream infile(fn.c_str(), ios::binary);
	if(!infile.good())
		return -1;

	infile.seekg(0, ios::end);
	int len = infile.tellg();
	infile.seekg(0);
	char * buf = new char[len+1];
	if(buf == NULL)
		return -1;

	infile.read(buf, len);
	content.assign(buf, len);
	delete[] buf;

	return 0;
}

int ReadFile(const string & fn, char ** content, unsigned long * len)
{
	if(content == NULL || len == NULL)
		return -1;

	ifstream infile(fn.c_str(), ios::binary);
	if(!infile.good())
		return -1;

	infile.seekg(0, ios::end);
	*len = infile.tellg();
	infile.seekg(0);
	*content = new char[*len+1];
	if(*content == NULL)
		return -1;

	infile.read(*content, *len);

	return 0;
}

int WriteFile(const string & fn, string & content)
{
	FILE * fp;
	fp = fopen(fn.c_str(), "wb");
	if(fp == NULL)
		return -1;

	fwrite(content.data(), content.size(), 1, fp);
	fclose(fp);

	return 0;
}

int DeleteFile(const string & fn)
{
    FILE * fp;
    fp = fopen(fn.c_str(), "rb");
    if(fp == NULL)
        return -1;

    fclose(fp);

    unlink(fn.c_str());
    return 0;
}

int IsFileExist(const string & fn)
{
    FILE * fp;
    fp = fopen(fn.c_str(), "rb");
    if(fp == NULL)
        return -1;

    fclose(fp);

    return 0;
}

int GetFileExt(const string & fn, string & ext)
{
	string::size_type pos = fn.rfind(".");
	if(pos > 0 && fn.size()-pos-1 <= 7 && fn.size()-pos-1 >= 1)
	{
		ext = fn.substr(pos+1, fn.size()-pos-1);

		return 0;
	}
	else //not found
	{
		ext = "";

		return -1;
	}
}

//把秒1155282950转换为日期2006-08-11 15:55:50
string ChangeSecToDateTime(time_t sec)
{
	if(sec < 0)
		return "";

	struct tm * ptm;
	ptm = localtime((const time_t*)&sec);

	char dateTime[100] = { 0 };
	sprintf(dateTime, "%04d-%02d-%02d %02d:%02d:%02d",
		ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

	return dateTime;
}


//把日期2006-08-11 15:55:50转换为秒1155282950
//兼容不带秒的日期格式
time_t ChangeDateTimeToSec(const string & dateTime)
{
	struct tm ltm;
	int ret;

	//先尝试匹配带秒数的格式
	ret = sscanf(dateTime.c_str(), "%04d-%02d-%02d %02d:%02d:%02d", 
		&ltm.tm_year, &ltm.tm_mon, &ltm.tm_mday, &ltm.tm_hour, &ltm.tm_min, &ltm.tm_sec);

	if(ret != 6)
	{
		//尝试匹配不带秒数的格式
		ret = sscanf(dateTime.c_str(), "%04d-%02d-%02d %02d:%02d", 
			&ltm.tm_year, &ltm.tm_mon, &ltm.tm_mday, &ltm.tm_hour, &ltm.tm_min);

		if(ret != 5)
		{
			return -1;
		}

		ltm.tm_sec = 0;
	}

	ltm.tm_year -= 1900;
	ltm.tm_mon -= 1;

	return mktime(&ltm);
}

#if (defined(_WIN32) || defined(_WIN64))

int CompareGMT(string t1, string t2)
{
	if(t1.size() > 50 || t2.size() > 50)
		return -2;
	int year1, month1, day1, hour1, minite1, second1;
	int year2, month2, day2, hour2, minite2, second2;
	char strMonth[50] = { 0 };
	char monthArr[12][4]={ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	string::size_type pos = 0;
	pos = t1.find(',');
	if(pos != string::npos)
		t1 = t1.substr(pos+1);

	pos = t2.find(',');
	if(pos != string::npos)
		t2 = t2.substr(pos+1);

	if(sscanf(t1.c_str(), "%d %s %d %d:%d:%d", &day1, strMonth, &year1, &hour1, &minite1, &second1) != 6)
		return -2;

	month1 = -1;
	int i;
	for(i = 0; i < 12; i++)
	{
		if(stricmp(monthArr[i], strMonth) == 0)
		{
			month1 = i + 1;
			break;
		}
	}

	if(month1 == -1)
		return -2;

	if(sscanf(t2.c_str(), "%d %s %d %d:%d:%d", &day2, strMonth, &year2, &hour2, &minite2, &second2) != 6)
		return -2;

	month2 = -1;
	for(i = 0; i < 12; i++)
	{
		if(stricmp(monthArr[i], strMonth) == 0)
		{
			month2 = i + 1;
			break;
		}
	}

	if(month2 == -1)
		return -2;

	if(year1 > year2)
		return 1;
	if(year1 < year2)
		return -1;
	if(month1 > month2)
		return 1;
	if(month1 < month2)
		return -1;
	if(day1 > day2)
		return 1;
	if(day1 < day2)
		return -1;
	if(hour1 > hour2)
		return 1;
	if(hour1 < hour2)
		return -1;
	if(minite1 > minite2)
		return 1;
	if(minite1 < minite2)
		return -1;
	if(second1 > second2)
		return 1;
	if(second1 < second2)
		return -1;

	return 0;
}

int GetFileLastModifiedTime(string file, string & gmt)
{
	gmt = "";
	OFSTRUCT ofStruct;
	HANDLE hFile = CreateFileA(file.c_str(), GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile < 0)
		return -1;

	FILETIME ftCreate, ftModify, ftAccess;
	if(!GetFileTime((HANDLE)hFile, &ftCreate, &ftAccess, &ftModify))
	{
		CloseHandle((HANDLE)hFile);
		return -1;
	}

	CloseHandle((HANDLE)hFile);

	time_t tm;
	ULARGE_INTEGER ui; 
	ui.LowPart = ftModify.dwLowDateTime; 
	ui.HighPart = ftModify.dwHighDateTime; 
	tm = ((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000); 

	char szBuf[50] = { 0 };
	strftime(szBuf , sizeof(szBuf) , "%a, %d %b %Y %H:%M:%S GMT" , gmtime( &tm ) );

	gmt = szBuf;

	return 0;
}

bool ConvertFileTimeToSystemTime(FILETIME ft, SYSTEMTIME & st)
{
	FILETIME ftLocal;
	FileTimeToLocalFileTime(&ft, &ftLocal);
	FileTimeToSystemTime(&ftLocal, &st);
	return true;
}

bool ConvertSystemTimeToFileTime(SYSTEMTIME st, FILETIME ft)
{
	FILETIME ftLocal;
	SystemTimeToFileTime(&st, &ftLocal);
	LocalFileTimeToFileTime(&ftLocal, &ft);
	return true;
}

bool ConvertGmtToFileTime(string gmt, FILETIME & ft)
{
	int year, month, day, hour, minute, second;
	char strMonth[50] = { 0 };
	char monthArr[12][4]={ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	string::size_type pos = 0;
	pos = gmt.find(',');
	if(pos != string::npos)
		gmt = gmt.substr(pos+1);

	if(sscanf(gmt.c_str(), "%d %s %d %d:%d:%d", &day, strMonth, &year, &hour, &minute, &second) != 6)
		return -1;

	month = -1;
	int i;
	for(i = 0; i < 12; i++)
	{
		if(stricmp(monthArr[i], strMonth) == 0)
		{
			month = i + 1;
			break;
		}
	}

	if(month == -1)
		return -1;

	struct tm lt;
	lt.tm_year = year - 1900;
	lt.tm_mon = month - 1;
	lt.tm_mday = day;
	lt.tm_hour = hour;
	lt.tm_min = minute;
	lt.tm_sec = second;
	lt.tm_isdst = 0;

	time_t t = mktime(&lt);

	ULARGE_INTEGER ui; 
	ui.QuadPart = (LONGLONG)t * 10000000 + 116444736000000000;

	ft.dwLowDateTime = ui.LowPart;
	ft.dwHighDateTime = ui.HighPart;

	return 0;
}

bool ConvertGmtToSystemTime(string gmt, SYSTEMTIME & st)
{
	FILETIME ft, ftLocal;
	ConvertGmtToFileTime(gmt, ft);

	FileTimeToLocalFileTime(&ft, &ftLocal);
	FileTimeToSystemTime(&ftLocal, &st);

	return 0;
}

bool ConvertGmtToTime(string gmt, time_t & t)
{
	int year, month, day, hour, minute, second;
	char strMonth[50] = { 0 };
	char monthArr[12][4]={ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	string::size_type pos = 0;
	pos = gmt.find(',');
	if(pos != string::npos)
		gmt = gmt.substr(pos+1);

	if(sscanf(gmt.c_str(), "%d %s %d %d:%d:%d", &day, strMonth, &year, &hour, &minute, &second) != 6)
		return -1;

	month = -1;
	int i;
	for(i = 0; i < 12; i++)
	{
		if(stricmp(monthArr[i], strMonth) == 0)
		{
			month = i + 1;
			break;
		}
	}

	if(month == -1)
		return -1;

	struct tm lt;
	lt.tm_year = year - 1900;
	lt.tm_mon = month - 1;
	lt.tm_mday = day;
	lt.tm_hour = hour;
	lt.tm_min = minute;
	lt.tm_sec = second;
	lt.tm_isdst = 0;

	t = mktime(&lt);

	return 0;
}

int SetFileWriteTime(string fn, FILETIME * lpLastWriteTime)
{
	HANDLE hFile;
	hFile = CreateFileA(fn.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
		return -1;

	int ret = -1;
	if(SetFileTime(hFile, NULL, NULL, lpLastWriteTime))
		ret = 0;
	CloseHandle(hFile);

	return 0;
}

#endif

int64 GetMilSec()
{
	#if (defined(_WIN32) || defined(_WIN64))

	return clock();

	#else

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);

	return tv.tv_sec*1000+tv.tv_usec/1000;

	#endif
}

int CompareVersion(const string & v1, const string & v2)
{
	if(v1.size() == 0 && v2.size() == 0)
		return 0;
	if(v1.size() == 0)
		return -1;
	if(v2.size() == 0)
		return 1;

	string::size_type p1 = 0, p2 = 0;
	while(true)
	{
		int n1 = atoi(v1.c_str() + p1);
		int n2 = atoi(v2.c_str() + p2);
		if(n1 > n2)
			return 1;
		if(n1 < n2)
			return -1;
		
		p1 = v1.find(".", p1);
		p2 = v2.find(".", p2);

		if(p1 == string::npos && p2 == string::npos)
			return 0;
		if(p1 == string::npos)
			return -1;
		if(p2 == string::npos)
			return 1;

		p1 += 1;
		p2 += 1;
	}
}


string GetDateTime()
{
    char date_time[100] = { 0 };

#if (defined(_WIN32) || defined(_WIN64))
    SYSTEMTIME CurTime;
    GetSystemTime(&CurTime);
    GetLocalTime(&CurTime);
    sprintf(date_time, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        int(CurTime.wYear), int(CurTime.wMonth), int(CurTime.wDay), int(CurTime.wHour), int(CurTime.wMinute), int(CurTime.wSecond), int(CurTime.wMilliseconds));
#else
    struct tm ptm;
    struct timeval tmstart;
    //time_t second = time(NULL);
    //ptm = localtime(((const time_t*)&second));
    //localtime_r(((const time_t*)&second), &ptm);
    gettimeofday(&tmstart, NULL);
    localtime_r(&(tmstart.tv_sec), &ptm);

    sprintf(date_time, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
        int(ptm.tm_year+1900), int(ptm.tm_mon+1), int(ptm.tm_mday), int(ptm.tm_hour), int(ptm.tm_min), int(ptm.tm_sec), int(tmstart.tv_usec));
#endif


    string s(date_time);
    return s;
}

string ConvertDate(const string & date)
{
	if(date.size() > 20)
		return "";

	char month[12][4]={ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	int y;
	char m[21] = { 0 };
	int d;

	sscanf(date.c_str(), "%s %d %d", m, &d, &y);

	int iMonth = -1;

	int i;
	for(i = 0; i < 12; i++)
	{
		if(stricmp(month[i], m) == 0)
		{
			iMonth = i + 1;
			break;
		}
	}

	if(y < 0 || iMonth == -1 || d < 1 || d > 31)
		return "";

	char sDate[20] = { 0 };
	snprintf(sDate, sizeof(sDate), "%d-%02d-%02d", y, iMonth, d);

	return sDate;
}

int ReadUTF8CharsByCount(const char * str, const char * start, char * chs, int count)
{
	if(str == NULL || start == NULL || chs == NULL || count < 0)
		return -1;

	const char * p = start;

	if((*p & 0x80) != 0x00)
	{
		while((*p & 0xC0) == 0x80 && p > str)
		{
			p--;
		}
	}

	int readCount = 0;
	int readLen = 0;
	while(readCount < count && *p != '\0')
	{
		if((*p & 0x80) == 0x00)
		{
			chs[readLen] = *p;
			p += 1;
			readLen += 1;
			readCount += 1;
		}
		else
		{
			int nBytes;
			if((*p & 0xFC) == 0xFC)
				nBytes = 6;
			else if((*p & 0xF8) == 0xF8)
				nBytes = 5;
			else if((*p & 0xF0) == 0xF0)
				nBytes = 4;
			else if((*p & 0xE0) == 0xE0)
				nBytes = 3;
			else if((*p & 0xC0) == 0xC0)
				nBytes = 2;
			else
				return -1;

			memcpy(chs+readLen, p, nBytes);
			p += nBytes;
			readLen += nBytes;
			readCount += 1;
		}
	}
	chs[readLen] = '\0';

	return readCount;
}

int ReadUTF8CharsByCountReverse(const char * str, const char * start, char * chs, int count)
{
	if(str == NULL || start == NULL || chs == NULL || count < 0)
		return -1;

	const char * p = start;

	if((*p & 0x80) != 0x00)
	{
		while((*(p+1) & 0xC0) == 0x80 && *(p+1) != '\0')
		{
			p++;
		}
	}

	int readCount = 0;
	int readLen = 0;
	while(readCount < count && p >= str)
	{
		//unsigned char chr = *p;
		if((*p & 0x80) == 0x00)
		{
			p -= 1;
			readLen += 1;
			readCount += 1;
		}
		else
		{
			int nBytes = 0;
			while((*p & 0xC0) == 0x80 && p > str)
			{
				p -= 1;
				nBytes++;
			}
			if((*p & 0xC0) == 0xC0 && p >= str)
			{
				p -= 1;
				nBytes++;
			}
			else
			{
				return -1;
			}
			
			readLen += nBytes;
			readCount += 1;
		} 
	}

	p += 1;

	memcpy(chs, p, readLen);
	chs[readLen] = '\0';

	return readLen;
}


#define IS_BLANK_CH(ch) (ch == ' '|| ch == '\t' || ch == '\r' || ch == '\n')

char * UrlTrim(char * url)
{
	if(url == NULL)
		return NULL;

	int start, end;
	start = 0;
	end = strlen(url) - 1;
	if(end < 0)
		return url;

	char * tmpstr = strdup(url);
	if(tmpstr == NULL)
		return NULL;

	while(end >= 0 && 
		(tmpstr[end] == ' ' || tmpstr[end] == '\t' || tmpstr[end] == '\r' || tmpstr[end] == '\n'
			|| tmpstr[end] == '?' || tmpstr[end] == '/' || tmpstr[end] == '\\' || tmpstr[end] == '#'))
	{
		tmpstr[end] = '\0';
		end --;
	}

	while(tmpstr[start] == ' ' || tmpstr[start] == '\t' || tmpstr[start] == '\r' || tmpstr[start] == '\n')
	{
		start ++;
	}

	//remove &#xxx
	char * p;
	p = strstr(tmpstr, "&#");
	if(p != NULL)
		p[0] = '\0';

	//remove #xxx
	p = strstr(tmpstr, "#");
	if(p != NULL)
		p[0] = '\0';

	strcpy(url, tmpstr+start);
	free(tmpstr);

	return url;
}

//提取出页面中的herf, embed, frame, iframe, img中的链接
//连接长度不超过1024
int ExtractLink(const string & src, map<string, string> & linkSet)
{
	linkSet.clear(); 
	if(src.size() < 10)
		return 0;

	const char * htmlStr = src.c_str();
	int srcLen = src.size();

	int count = 0;
	
	int bracketMark = 0;
	int anchorMark = 0, hrefMark = 0; // for <a href = 'xxx'>
	int embebMark = 0, embedSrcMark = 0; //for <embed src = 'xxx'>
	int frameMark = 0, frameSrcMark = 0; //for <frame src = 'xxx'> or <iframe src = 'xxx'>
	int imgMark = 0, imgSrcMark = 0; //for <img src = "xxx">

	int i = 0;
	//int j = 0;
	//while(htmlStr[i] != '\0')
	while(i < srcLen)
	{
		if(htmlStr[i] == '\0')
			break;
	
		if(htmlStr[i] == '<') //match bracket
		{
			bracketMark = 1;
			i++;
		}
		
		// for <a>
		else if((htmlStr[i] == 'a' || htmlStr[i] == 'A') //match anchor
			//&& (htmlStr[i+1] == ' ' || htmlStr[i+1] == '\t')
			&& IS_BLANK_CH(htmlStr[i+1])
			&& bracketMark == 1)
		{
			anchorMark = 1;
			i += 2;
		}
		else if(strncasecmp(htmlStr+i, "href", 4) == 0 //match herf
			//&& (htmlStr[i+4] == ' ' || htmlStr[i+4] == '\t' || htmlStr[i+4] == '=')
			&& (IS_BLANK_CH(htmlStr[i+4]) || htmlStr[i+4] == '=')
			&& (bracketMark == 1 && anchorMark == 1))
		{
			hrefMark = 1;
			i += 5;
		}
		
		//for <embed>
		else if(strncasecmp(htmlStr+i, "embed", 5) == 0 //match embed
			//&& (htmlStr[i+5] == ' ' || htmlStr[i+5] == '\t')
			&& IS_BLANK_CH(htmlStr[i+5])
			&& bracketMark == 1)
		{
			embebMark = 1;
			i += 6;
		}
		else if(strncasecmp(htmlStr+i, "src", 3) == 0 //match src
			//&& (htmlStr[i+3] == ' ' || htmlStr[i+3] == '\t' || htmlStr[i+3] == '=')
			&& (IS_BLANK_CH(htmlStr[i+3]) || htmlStr[i+3] == '=')
			&& (bracketMark == 1 && embebMark == 1))
		{
			embedSrcMark = 1;
			i += 4;
		}

		//for <frame> <iframe>
		else if(strncasecmp(htmlStr+i, "frame", 5) == 0 //match frame
			//&& (htmlStr[i+5] == ' ' || htmlStr[i+5] == '\t')
			&& IS_BLANK_CH(htmlStr[i+5])
			&& bracketMark == 1)
		{
			frameMark = 1;
			i += 6;
		}
		else if(strncasecmp(htmlStr+i, "iframe", 6) == 0 //match iframe
			//&& (htmlStr[i+6] == ' ' || htmlStr[i+6] == '\t')
			&& IS_BLANK_CH(htmlStr[i+6])
			&& bracketMark == 1)
		{
			frameMark = 1;
			i += 7;
		}
		else if(strncasecmp(htmlStr+i, "src", 3) == 0 //match src
			//&& (htmlStr[i+3] == ' ' || htmlStr[i+3] == '\t' || htmlStr[i+3] == '=')
			&& (IS_BLANK_CH(htmlStr[i+3]) || htmlStr[i+3] == '=')
			&& (bracketMark == 1 && frameMark == 1))
		{
			frameSrcMark = 1;
			i += 4;
		}

		//for <img>
		else if(strncasecmp(htmlStr+i, "img", 3) == 0 //match embed
			//&& (htmlStr[i+3] == ' ' || htmlStr[i+3] == '\t')
			&& IS_BLANK_CH(htmlStr[i+3])
			&& bracketMark == 1)
		{
			imgMark = 1;
			i += 4;
		}
		else if(strncasecmp(htmlStr+i, "src", 3) == 0 //match src
			//&& (htmlStr[i+3] == ' ' || htmlStr[i+3] == '\t' || htmlStr[i+3] == '=')
			&& (IS_BLANK_CH(htmlStr[i+3]) || htmlStr[i+3] == '=')
			&& (bracketMark == 1 && imgMark == 1))
		{
			imgSrcMark = 1;
			i += 4;
		}

		//忽略空白字符
		//注意，此处的空白字符是在未发现url时的空白字符
		//发现url之后，其中的空白字符会由提取url的代码处理，并不在此处处理。
		//else if(htmlStr[i] == ' ' || htmlStr[i] == '\t') //match space 
		else if(IS_BLANK_CH(htmlStr[i])) //match space 
		{
			i++;
		}
		
		//以<a name='fff' uyo='qgr' href = 'xxxx'>为例
		//忽略"<a"和"href"之间的语句
		else if((bracketMark && (anchorMark || embebMark || frameMark || imgMark))
			&& (htmlStr[i] != '>'))
		{
			i++;
		}
		else//if is another letter, set all mark to 0
		{
			bracketMark = 0;
			anchorMark = 0;
			hrefMark = 0;
			embebMark = 0;
			embedSrcMark = 0;
			frameMark = 0;
			frameSrcMark = 0;
			imgMark = 0;
			imgSrcMark = 0;
			
			i++;
		}

		if(i >= srcLen)
			break;

		//if bracketMark anchorMark hrefMark are be set to 1, then we matched a url
		if(bracketMark && ((anchorMark && hrefMark) || (embebMark && embedSrcMark) || (frameMark && frameSrcMark) || (imgMark && imgSrcMark)))
		{
			//当url被引号引起来后，就不能以空格做为结束标记
			int qoutemark = 0;
			//int start = j;
			int j = 0;
			char link[2*1024+1];
			char linktitle[2*1024+1];
			while(htmlStr[i] != '\0' 
				//&& (htmlStr[i] == ' ' || htmlStr[i] == '\t' || htmlStr[i] == '=' 
				&& (IS_BLANK_CH(htmlStr[i]) || htmlStr[i] == '=' 
				|| htmlStr[i] == '"'  || htmlStr[i] == '\''))
			{
				if(htmlStr[i] == '"'  || htmlStr[i] == '\'') //被引号引了起来
					qoutemark = 1;
				i++;
			}

			
			/*while(htmlStr[i] != '\0' 
				&& htmlStr[i] != '"'  && htmlStr[i] != '\'' && !(!qoutemark && htmlStr[i] == ' ' )
				&& !(!qoutemark && htmlStr[i] == '\t') && htmlStr[i] != '>' && htmlStr[i] != '<')*/

			//提取url
			//当没有引号时，遇到空白字符会结束
			//有引号时，空格将保留，其它空白字符将忽略
			//url的#参数被过滤掉
			while(htmlStr[i] != '\0' 
				&& htmlStr[i] != '"'  && htmlStr[i] != '\'' && !(!qoutemark && IS_BLANK_CH(htmlStr[i]))
				//&& htmlStr[i] != '\t' && htmlStr[i] != '>' && htmlStr[i] != '<'
				&& htmlStr[i] != '>' && htmlStr[i] != '<'
				&& !(htmlStr[i-1] != '&' && htmlStr[i] == '#'))
			{
				if(IS_BLANK_CH(htmlStr[i]) && htmlStr[i] != ' ') //非空格的空白字符被忽略
				{
					i++;
					continue;
				}
				
				link[j++] = htmlStr[i++];
				if(j >= 1024)
					break;
			}
			link[j] = '\0';
			UrlTrim(link);

			//如果是href格式，则断续寻找title
			//title提取不要求十分密，所以，有可能提取不到，或提取错误
			j = 0;
			if(hrefMark)
			{
				//先找到<a>的'>'处
				while(htmlStr[i] != '\0' && htmlStr[i] != '>')
					i++;
				if(htmlStr[i] == '>')
				{
					i++;
					int ibak = i; //使扫苗指针能够回溯。因为<a>和</a>之间可能包含<img>等其它link
					while(htmlStr[i] != '\0')
					{
						//使多个空格只被保留一个
						if(IS_BLANK_CH(htmlStr[i]))
						{
							linktitle[j++] = ' ';
							i++;
							if(j >= 1024)
								break;
						}
						while(IS_BLANK_CH(htmlStr[i]))
							i++;

						if(htmlStr[i] == '\0') 
							break;

						//跳过可以能的tag，如<span> <b> <font> <img>
						//以"  <b>   <font color="AE355E">游戏下载</font></b>点击</a>为例
						while(htmlStr[i] != '\0') 
						{
							//如果为'可忽略tag'，就跳过。注意：此处不严密
							if(strncasecmp(htmlStr+i, "<font", 5) == 0 || strncasecmp(htmlStr+i, "</font", 6) == 0
								|| strncasecmp(htmlStr+i, "<b", 2) == 0 || strncasecmp(htmlStr+i, "</b", 3) == 0
								|| strncasecmp(htmlStr+i, "<span", 5) == 0 || strncasecmp(htmlStr+i, "</span", 6) == 0
								|| strncasecmp(htmlStr+i, "<img", 4) == 0)
							{
								while(htmlStr[i] != '\0' && htmlStr[i] != '>')
									i++;
								if(htmlStr[i] == '>')
									i++;		
							}
							else
							{
								break;
							}
						}
						
						//在跳过可忽略的tag之后，遇到'<'就必须停下来
						if(htmlStr[i] == '\0' || htmlStr[i] == '<')
							break;

						linktitle[j++] = htmlStr[i++];
						if(j >= 1024)
							break;
					}

					i = ibak;
				}
			}
			linktitle[j] = '\0';
			//StrTrim(linktitle);

			if(link[0] != '\0') //当url不为空时才有效
			{
				string slink = link;
				string slinktitle = linktitle;

				/*string_replace_substr(slink, "\r", " "); //把回车替换成空格
				string_replace_substr(slink, "\n", " ");
				while(string_replace_substr(slink, "  ", " ") > 0) //把多个空格替换为一个空格
				{
					//do nothing
				}

				string_replace_substr(slinktitle, "\r", " "); //把回车替换成空格
				string_replace_substr(slinktitle, "\n", " ");
				while(string_replace_substr(slinktitle, "  ", " ") > 0) //把多个空格替换为一个空格
				{
					//do nothing
				}*/

				StringTrim(slink);
				StringTrim(slinktitle);

				//string_replace_substr(slink, "\r\n", " "); //把回车替换成空格
				//string_replace_substr(slinktitle, "\r\n", " "); //把回车替换成空格
				StringReplace(slink, "&amp;", "&");

				linkSet.insert(map<string, string>::value_type(slink, slinktitle));
				
				/*string link_title;
				link_title = link;
				link_title += "\t";
				link_title += linktitle;

				linkSet.insert(link_title);*/
			}
			
			bracketMark = 0;
			anchorMark = 0;
			hrefMark = 0;
			embebMark = 0;
			embedSrcMark = 0;
			frameMark = 0;
			frameSrcMark = 0;
			imgMark = 0;
			imgSrcMark = 0;
		}

	}

	return 0;
	
}

string GetMime(string ext)
{
	string mt[128][2] =
	{
		{ "3gp", "video/3gpp" },
		{ "asf", "video/x-ms-asf" },
		{ "asn", "application/astound" },
		{ "asp", "application/x-asap" },
		{ "asx", "video/x-ms-asf" },
		{ "au", "audio/basic" },
		{ "avi", "video/x-msvideo" },
		{ "cab", "application/vnd.ms-cab-compressed" },
		{ "conf", "text/plain" },
		{ "css", "text/css" },
		{ "dhtml", "text/html" },
		{ "doc", "application/msword" },
		{ "flv", "video/x-flv" },
		{ "gif", "image/gif" },
		{ "gps", "application/x-gps" },
		{ "gtar", "application/x-gtar" },
		{ "gz", "application/x-gzip" },
		{ "hdf", "application/x-hdf" },
		{ "hdml", "text/x-hdml" },
		{ "hdm", "text/x-hdml" },
		{ "html", "text/html" },
		{ "htm", "text/html" },
		{ "hts", "text/html" },
		{ "hts", "text/html" },
		{ "ico", "image/x-icon" },
		{ "ifm", "image/gif" },
		{ "ini", "text/plain" },
		{ "j2k", "image/j2k" },
		{ "jad", "text/vnd.sun.j2me.app-descriptor" },
		{ "jam", "application/x-jam" },
		{ "jar", "application/java-archive" },
		{ "jpeg", "image/jpeg" },
		{ "jpe", "image/jpeg" },
		{ "jpg", "image/jpeg" },
		{ "jpz", "image/jpeg" },
		{ "js", "application/x-javascript" },
		{ "log", "text/plain" },
		{ "mil", "image/x-cals" },
		{ "mio", "audio/x-mio" },
		{ "mng", "video/x-mng" },
		{ "mod", "audio/x-mod" },
		{ "movie", "video/x-sgi-movie" },
		{ "mov", "video/quicktime" },
		{ "mp2", "audio/mpeg" },
		{ "mp3", "audio/mpeg" },
		{ "mp4", "video/mp4" },
		{ "mpc", "application/vnd.mpohun.certificate" },
		{ "mpeg", "video/mpeg" },
		{ "mpe", "video/mpeg" },
		{ "mpg4", "video/mp4" },
		{ "mpga", "audio/mpeg" },
		{ "mpg", "video/mpeg" },
		{ "nbmp", "image/nbmp" },
		{ "nsnd", "audio/nsnd" },
		{ "pac", "audio/x-pac" },
		{ "pac", "audio/x-pac" },
		{ "pae", "audio/x-epac" },
		{ "pae", "audio/x-epac" },
		{ "pbm", "image/x-portable-bitmap" },
		{ "pbm", "image/x-portable-bitmap" },
		{ "pcx", "image/x-pcx" },
		{ "pcx", "image/x-pcx" },
		{ "pda", "image/x-pda" },
		{ "pda", "image/x-pda" },
		{ "pdf", "application/pdf" },
		{ "pict", "image/x-pict" },
		{ "png", "image/png" },
		{ "ppt", "application/vnd.ms-powerpoint" },
		{ "pvx", "video/x-pv-pvx" },
		{ "qcp", "audio/vnd.qcelp" },
		{ "qtif", "image/x-quicktime" },
		{ "qti", "image/x-quicktime" },
		{ "qt", "video/quicktime" },
		{ "r3t", "text/vnd.rn-realtext3d" },
		{ "ra", "audio/x-pn-realaudio" },
		{ "ram", "audio/x-pn-realaudio" },
		{ "rar", "application/x-rar-compressed" },
		{ "ras", "image/x-cmu-raster" },
		{ "rf", "image/vnd.rn-realflash" },
		{ "rgb", "image/x-rgb" },
		{ "rm", "application/vnd.rn-realmedia" },
		{ "rmf", "audio/x-rmf" },
		{ "rmm", "audio/x-pn-realaudio" },
		{ "rmvb", "application/vnd.rn-realmedia" },
		{ "rv", "video/vnd.rn-realvideo" },
		{ "s3m", "audio/x-mod" },
		{ "s3z", "audio/x-mod" },
		{ "sgml", "text/x-sgml" },
		{ "sgm", "text/x-sgml" },
		{ "si6", "image/si6" },
		{ "si7", "image/vnd.stiwap.sis" },
		{ "si9", "image/vnd.lgtwap.sis" },
		{ "smd", "audio/x-smd" },
		{ "smz", "audio/x-smd" },
		{ "snd", "audio/basic" },
		{ "svf", "image/vnd" },
		{ "svg", "image/svg-xml" },
		{ "svh", "image/svh" },
		{ "swf", "application/x-shockwave-flash" },
		{ "tiff", "image/tiff" },
		{ "tif", "image/tiff" },
		{ "txt", "text/plain" },
		{ "wav", "audio/x-wav" },
		{ "wax", "audio/x-ms-wax" },
		{ "wma", "audio/x-ms-wma" },
		{ "wmd", "application/x-ms-wmd" },
		{ "wmf", "application/x-msmetafile" },
		{ "wmlc", "application/vnd.wap.wmlc" },
		{ "wmlsc", "application/vnd.wap.wmlscriptc" },
		{ "wmlscript", "text/vnd.wap.wmlscript" },
		{ "wmls", "text/vnd.wap.wmlscript" },
		{ "wml", "text/vnd.wap.wml" },
		{ "wmv", "audio/x-ms-wmv" },
		{ "wm", "video/x-ms-wm" },
		{ "wmx", "video/x-ms-wmx" },
		{ "wmz", "application/x-ms-wmz" },
		{ "wpng", "image/x-up-wpng" },
		{ "xht", "application/xhtml+xml" },
		{ "xhtm", "application/xhtml+xml" },
		{ "xhtml", "application/xhtml+xml" },
		{ "xls", "application/vnd.ms-excel" },
		{ "xml", "text/xml" },
		{ "xsf", "text/xml" },
		{ "xsl", "text/xml" },
		{ "xslt", "text/xml" },
		{ "zac", "application/x-zaurus-zac" },
		{ "z", "application/x-compress" },
		{ "zip", "application/zip" }
	};

	if(ext.find("?") != string::npos) {
		ext = ext.substr(0, ext.find("?"));
	}
	if(ext.rfind(".") != string::npos) {
		ext = ext.substr(ext.rfind(".") + 1);
	}
	StringLower(ext);

	int i;
	for(i = 0; i < 128; i++) {
		if(mt[i][0] == ext) {
			return mt[i][1];
		}
	}

	return "application/octet-stream"; 
}

string GenGuid()
{
	static bool bInit = false;
	if(!bInit)
	{
		unsigned int nRandomFeed = time(NULL) + clock();
		srand(nRandomFeed);
		bInit = true;
	}

	char szGuid[37] = { 0 };
	_snprintf(szGuid, sizeof(szGuid),
		"%04X%04X-%04X-%04X-%04X-%04X%04X%04X",
		rand()%65536, rand()%65536, rand()%65536, rand()%65536, 
		rand()%65536, rand()%65536, rand()%65536, rand()%65536);

	return szGuid;
}

bool IsHexStr(string sHexStr)
{
	int i;
	for(i = 0; i < sHexStr.size(); i++)
	{
		if(!isxdigit(sHexStr[i]))
			return false;
	}

	return true;
}

bool IsGuidStr(string sGuidStr)
{
	if(sGuidStr.size() != 36)
		return false;

	int i;
	for(i = 0; i < 36; i++)
	{
		if(i == 8 || i == 13 || i == 18 || i == 23)
		{
			if(sGuidStr[i] != '-')
				return false;
		}
		else
		{
			if(!isxdigit(sGuidStr[i]))
				return false;
		}
	}

	return true; 
}
