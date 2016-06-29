#if (defined(_WIN32) || defined(_WIN64))
#include "stdafx.h"
#endif
#include "HttpMsg.h"
#include <sstream>
#include <stdarg.h>
#include <algorithm>
#include <time.h>
#include <ctype.h>

static int64 GetMilSec()
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

static string itoa64(int64 i) 
{
	char tmp[100];
	#if (defined(_WIN32) || defined(_WIN64))
	sprintf(tmp, "%I64d", i);
	#else
	sprintf(tmp, "%lld", i);
	#endif

	return string(tmp);
}

static int64 atoi64(string s) 
{
	int j = 0;
	while(j < s.size()) 
	{
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

static int StringReplace(string & str, const string & old_str, const string & new_str)
{
	string::size_type pos = 0;
	string::size_type oldlen = old_str.size();
	string::size_type newlen = new_str.size();
	int n = 0;
	while((pos=str.find(old_str, pos)) != string::npos)
	{
		str.replace(pos, oldlen, new_str);
		pos += newlen;
		n++;
	}

	return n;
}

static void StringTrim(string & text)
{
	int i = 0, j = text.size()-1;
	while(i <= j)
	{
		if(text[i] == ' ' || text[i] == '\t' || text[i] == '\r' || text[i] == '\n')
		{
			i++;
			continue;
		}
		if(text[j] == ' ' || text[j] == '\t' || text[j] == '\r' || text[j] == '\n')
		{
			j--;
			continue;
		}

		break;
	}

	if(i == 0 && j == text.size()-1) 
	{
		return;
	}

	if(i <= j)
	{
		text = text.substr(i, j-i+1);
	}
	else
	{
		text = "";
	}
}

static int StringSplit(const string & str, vector<string> & arr, char delim)
{
	arr.clear();

	stringstream sstr(str);
	string s;
	while(getline(sstr, s, delim))
	{
		arr.push_back(s);
	}

	return arr.size();
}

static string StringPrintf(const char * format, ...)
{
	if(format[0] == '\0')
		return string("");

	char s[2049] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(s, sizeof(s), format, args);
    va_end(args);

	return string(s);
}

static int StringLower(string & str)
{
	int i;
	for(i = 0; i < str.size(); i++)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}

	return 0;
}

static int StringUpper(string & str)
{
	int i;
	for(i = 0; i < str.size(); i++)
	{
		if(str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
	}

	return 0;
}

int GetHTTPRequestCmd(string head, string & cmd)
{
	cmd = "";
	if(strncmp(head.c_str(), "GET ", 4) == 0)
		cmd = "GET";
	else if(strncmp(head.c_str(), "POST ", 5) == 0)
		cmd = "POST";
	else if(strncmp(head.c_str(), "HEAD ", 5) == 0)
		cmd = "HEAD";
	else
		return -1;

	return 0;
}

int GetHTTPStatusCode(string head, string & code)
{
	code = "";
	string::size_type p1, p2;
	p1 = head.find(" ");
	if(p1 == string::npos)
		return -1;
	p1 += 1;

	p2 = head.find(" ", p1);
	if(p2 == string::npos || p2 - p1 != 3)
		return -1;
	
	code = head.substr(p1, p2 - p1);

	return 0;
}

int SetHTTPStatusCode(string & head, string code)
{
	if(code.size() != 3)
		return -1;

	string::size_type p1, p2;
	p1 = head.find(" ");
	if(p1 == string::npos)
		return -1;
	p1 += 1;

	p2 = head.find(" ", p1);
	if(p2 == string::npos || p2 - p1 != 3)
		return -1;
	
	head.replace(p1, p2 - p1, code);

	return 0;
}

int GetHTTPPath(string head, string & path)
{
	//GET /index.html HTTP/1.1
	path = "";
	string::size_type p1, p2;
	p1 = head.find(" ");
	p2 = head.find(" HTTP");
	if(p1 == string::npos || p2 == string::npos)
	{
		return -1;
	}
	p1 += 1;

	path = head.substr(p1, p2 - p1);

	return 0;
}

int SetHTTPPath(string & head, string path)
{
	//GET /index.html HTTP/1.1
	//path = EncodePath(path);

	if(path.size() == 0)
		path = "/";

	string::size_type p1, p2;
	p1 = head.find(" ");
	p2 = head.find(" HTTP");
	if(p1 == string::npos || p2 == string::npos)
	{
		return -1;
	}
	p1 += 1;

	head.replace(p1, p2 - p1, path);

	return 0;
}

int GetHTTPHeader(string head, string name, string & value)
{
	value = "";

	while(name.size() > 0)
	{
		if(name[0] == ' ')
		{
			name.erase(0, 1);
			continue;
		}
		if(name[name.size()-1] == ':' || name[name.size()-1] == ' ')
		{
			name.erase(name.size()-1, 1);
			continue;
		}
		
		break;
	}

	if(name.size() == 0)
		return -1;

	string::size_type p1, p2;
	p1 = head.find("\r\n" + name + ":");
	if(p1 == string::npos)
		return -1;
	p1 += name.size() + 3;
	while(head[p1] == ' ')
	{
		p1++;
	}

	p2 = head.find("\r\n", p1);
	if(p2 == string::npos)
		return -1;

	value = head.substr(p1, p2 - p1);

	return 0;
}

int SetHTTPHeader(string & head, string name, string value)
{
	while(name.size() > 0)
	{
		if(name[0] == ' ')
		{
			name.erase(0, 1);
			continue;
		}
		if(name[name.size()-1] == ':' || name[name.size()-1] == ' ')
		{
			name.erase(name.size()-1, 1);
			continue;
		}
		
		break;
	}

	if(name.size() == 0)
		return -1;

	string::size_type p1, p2;
	p1 = head.find("\r\n" + name + ":");
	if(p1 != string::npos)
	{
		p1 += name.size() + 3;
		while(head[p1] == ' ')
		{
			p1++;
		}

		p2 = head.find("\r\n", p1);
	}

	if(p1 != string::npos && p2 != string::npos)
	{
		head.replace(p1, p2 - p1, value);
		return 0;
	}
	else
	{
		p1 = head.find("\r\n\r\n");
		if(p1 != string::npos)
		{
			head.replace(p1, 0, "\r\n" + name + ": " + value);
			return 0;
		}

		return -1;
	}
}

int RemoveHTTPHeader(string & head, string name)
{
	while(name.size() > 0)
	{
		if(name[0] == ' ')
		{
			name.erase(0, 1);
			continue;
		}
		if(name[name.size()-1] == ':' || name[name.size()-1] == ' ')
		{
			name.erase(name.size()-1, 1);
			continue;
		}
		
		break;
	}

	if(name.size() == 0)
		return -1;

	string::size_type p1, p2;
	p1 = head.find("\r\n" + name + ":");
	if(p1 == string::npos)
		return -1;
	p1 += 2;

	p2 = head.find("\r\n", p1);
	if(p2 == string::npos)
		return -1;

	head.replace(p1, p2 - p1 + 2, "");

	return 0;
}

int GetHTTPHeadLength(string head)
{
	string::size_type p = head.find("\r\n\r\n");
	if(p == string::npos)
		return -1;
	
	return p + 4;
}

int RecvHTTPRequestHead(int sock, string & head, int timeout)
{
	int64 startTime = GetMilSec();
	head = "";
	while(true)
	{
		char buf[1] = { 0 };
		int len = recv(sock, buf, 1, 0);
		if(len < 0)
		{
			#if (defined(_WIN32) || defined(_WIN64))
			
			if(GetLastError() == 10035)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
			#else
			
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				usleep(10*1000);
				continue;
			}
			else
			{
				return -1;
			}

			#endif
		}
		else if(len == 0)
		{
			Sleep(1);
			continue;
		}

		head.append(buf, len);

		//判断消息头是否接收完成
		if(head.find("\r\n\r\n") != string::npos)
		{
			if(strncmp(head.c_str(), "GET ", 4) != 0 && strncmp(head.c_str(), "POST ", 5) != 0 && strncmp(head.c_str(), "HEAD ", 5) != 0)
				return -1;
			return 0;
		}

		if(timeout >= 0 && GetMilSec() - startTime >= timeout)
		{
			closesocket(sock);
			return -1;
		}
	}
}

int RecvHTTPResponseHead(int sock, string & head, int timeout)
{
	int64 startTime = GetMilSec();
	head = "";
	while(true)
	{
		char buf[1] = { 0 };
		int len = recv(sock, buf, 1, 0);
		if(len < 0)
		{
			#if (defined(_WIN32) || defined(_WIN64))
			
			if(GetLastError() == 10035)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
			#else
			
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				usleep(10*1000);
				continue;
			}
			else
			{
				return -1;
			}

			#endif
		}
		else if(len == 0)
		{
			Sleep(1);
			continue;
		}

		head.append(buf, len);

		//判断消息头是否接收完成
		if(head.find("\r\n\r\n") != string::npos)
		{
			if(strncmp(head.c_str(), "HTTP/", 5) != 0)
				return -1;
			return 0;
		}

		if(timeout >= 0 && GetMilSec() - startTime >= timeout)
		{
			closesocket(sock);
			return -1;
		}
	}
}

int DecodeChunk(string str, string & out)
{
	out = "";
	string::size_type offset = 0;
	while(true)
	{
		int chunkSize = strtol(str.c_str()+offset, NULL, 16);
		offset = str.find("\r\n", offset);
		if(offset == string::npos)
			return -1;
		if(chunkSize == 0)
			return 0;
		offset += 2;
		if(str.size()-offset < chunkSize+2)
			return -1;
		out += str.substr(offset, chunkSize);
		offset += chunkSize + 2;
	}

	return 0;
}

CFormArgs::CFormArgs()
{
}

int CFormArgs::Dump(string & str)
{
	map<string, string>::iterator it;
	for(it = m_args.begin(); it != m_args.end(); it++)
	{
		str += it->first + " = " + it->second;
		str += "\n";
	}

	return 0;
}

int CFormArgs::Encode(string & str, REQ_METHOD reqMethod)
{
	str = "";
	
	//if(m_args.size() == 0)
	//	return 0;

	if(reqMethod == METHOD_POST_MULTIPART) 
	{
		map<string, string>::iterator it;
		int boundary_ok = 1;
		do
		{
			m_boundary = "--------";
			char rc[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			int i;
			for(i = 0; i < 32; i++) 
			{
				m_boundary.append(1, rc[rand()%36]);
			}
			
			boundary_ok = 1;
			for(it = m_args.begin(); it != m_args.end(); it++)
			{
				if(it->first.find(m_boundary) != string::npos || it->second.find(m_boundary) != string::npos) 
				{
					boundary_ok = 0;
					break;
				}
			}
		} while(!boundary_ok);
		
		for(it = m_args.begin(); it != m_args.end(); it++)
		{
			str += m_boundary + "\r\n";
			str += "Content-Disposition: form-data; name=\"" + it->first + "\"" + "\r\n\r\n";
			str += it->second + "\r\n";
		}
		str += m_boundary + "--\r\n";
	}
	else
	{
		map<string, string>::iterator it;
		for(it = m_args.begin(); it != m_args.end(); it++)
		{
			str += it->first  + "=" + EncodeVal(it->second) + "&";
		}
		if(str.size() > 0)
			str.erase(str.size()-1);
	}

	return 0;
}

int CFormArgs::Decode(const string & args, string method, int type) 
{
	Clear();

	if(args.size() == 0)
		return 0;

	if(type == -1)
	{
		if(args[0] == '-')
		{
			type = 1;
		}
		else
		{
			type = 0;
		}
	}
	
	if(type == 0)
	{
		vector<string> vec_args;
		StringSplit(args, vec_args, '&');

		int i;
		for(i = 0; i < vec_args.size(); i++)
		{
			vector<string> arg_val;
			StringSplit(vec_args[i], arg_val, '=');
			if(arg_val.size() >= 2)
			{
				string val;
				int j;
				for(j = 0; j < arg_val[1].size(); j++) 
				{
					if(arg_val[1][j] == '%')
					{
						if(j+2 < arg_val[1].size())
						{
							unsigned char ch = 0;
							if(arg_val[1][j+1] >= '0' && arg_val[1][j+1] <= '9')
								ch = (arg_val[1][j+1]-'0') * 16;
							else if(arg_val[1][j+1] >= 'a' && arg_val[1][j+1] <= 'f')
								ch = (arg_val[1][j+1]-'a'+10) * 16;
							else if(arg_val[1][j+1] >= 'A' && arg_val[1][j+1] <= 'F')
								ch = (arg_val[1][j+1]-'A'+10) * 16;
							else
							{
								ch = arg_val[1][j+1];
								val.append(1, ch);
								j += 1;
								continue;
							}

							if(arg_val[1][j+2] >= '0' && arg_val[1][j+2] <= '9')
								ch += (arg_val[1][j+2]-'0');
							else if(arg_val[1][j+2] >= 'a' && arg_val[1][j+2] <= 'f')
								ch += (arg_val[1][j+2]-'a'+10);
							else if(arg_val[1][j+2] >= 'A' && arg_val[1][j+2] <= 'F')
								ch += (arg_val[1][j+2]-'A'+10);

							val.append(1, ch);

							j += 2;
						}
					}
					else if(arg_val[1][j] == '+')
					{
						val.append(1, ' ');
					}
					else
					{
						val.append(1, arg_val[1][j]);
					}
				}
				
				string name = arg_val[0];
				//StringLower(name);
				//m_args.insert(map<string, string>::value_type(name, val));
				SetArg(name, val);
			}
			else if(arg_val.size() == 1 && vec_args[i].find("=") != string::npos)
			{
				string name = arg_val[0];
				//StringLower(name);
				//m_args.insert(map<string, string>::value_type(name, ""));
				SetArg(name, "");
			}
		}

		return 0;
	}
	else 
	{
		size_t pos = args.find("\r\n");
		string boundary;
		if(pos != string::npos)
		{
			boundary = args.substr(0, pos);
			pos = boundary.size()+2;
			size_t pos2;
			while((pos2 = args.find(boundary, pos)) != string::npos)
			{
				string data = args.substr(pos, pos2-2-pos);

				pos = data.find("name=\"");
				if(pos == string::npos)
					return -1;
				pos += 6;
				char arg[100];
				memset(arg, 0, sizeof(arg));
				int i = 0;
				while(pos < data.size()-1 && data[pos] != '"' && i < sizeof(arg)-1)
				{
					arg[i] = data[pos];
					i++;
					pos++;
				}

				pos = data.find("\r\n\r\n");
				if(pos == string::npos)
					return -1;
				pos += 4;
				string val = data.substr(pos);
				
				//m_args.insert(map<string, string>::value_type(arg, val));
				SetArg(arg, val);

				pos = pos2 + boundary.size()+2;
			}
		}
		else
		{
			return -1;
		}
	}

	return 0;
}

int CFormArgs::SetArg(const string & argname, string argvalue)
{
	/*
	string tmpArgname = argname;
	StringLower(tmpArgname);

	map<string, string>::iterator it;
	it = m_args.find(tmpArgname);
	if(it != m_args.end())
		m_args.erase(it);
	*/

	map<string, string>::iterator it;
	for(it = m_args.begin(); it != m_args.end(); it++)
	{
		if(strcasecmp(argname.c_str(), it->first.c_str()) == 0)
		{
			m_args.erase(it);
			break;
		}
	}

	m_args.insert(map<string, string>::value_type(argname, argvalue));

	return 0;
}

int CFormArgs::GetArg(const string & argname, string & argvalue)
{
	/*
	string tmpArgname = argname;
	StringLower(tmpArgname);

	argvalue = "";

	map<string, string>::iterator it;
	it = m_args.find(tmpArgname);
	*/

	argvalue = "";

	map<string, string>::iterator it;
	for(it = m_args.begin(); it != m_args.end(); it++)
	{
		if(strcasecmp(argname.c_str(), it->first.c_str()) == 0)
		{
			break;
		}
	}
	
	if(it == m_args.end())
		return -1;

	argvalue = it->second;

	return 0;
}

int CFormArgs::GetArg(const string & argname, int & argvalue)
{
	/*
	string tmpArgname = argname;
	StringLower(tmpArgname);

	map<string, string>::iterator it;
	it = m_args.find(tmpArgname);
	*/

	map<string, string>::iterator it;
	for(it = m_args.begin(); it != m_args.end(); it++)
	{
		if(strcasecmp(argname.c_str(), it->first.c_str()) == 0)
		{
			break;
		}
	}
	
	if(it == m_args.end())
		return -1;

	argvalue = atoi(it->second.c_str());

	return 0;
}

int CFormArgs::SetArg(const string & argname, int64 argvalue)
{
	/*
	string tmpArgname = argname;
	StringLower(tmpArgname);

	map<string, string>::iterator it;
	it = m_args.find(tmpArgname);
	if(it != m_args.end())
		m_args.erase(it);

	m_args.insert(map<string, string>::value_type(tmpArgname, itoa64(argvalue)));
	*/

	map<string, string>::iterator it;
	for(it = m_args.begin(); it != m_args.end(); it++)
	{
		if(strcasecmp(argname.c_str(), it->first.c_str()) == 0)
		{
			m_args.erase(it);
			break;
		}
	}

	m_args.insert(map<string, string>::value_type(argname, itoa64(argvalue)));

	return 0;
}

int CFormArgs::GetArg(const string & argname, int64 & argvalue)
{
	/*
	string tmpArgname = argname;
	StringLower(tmpArgname);

	map<string, string>::iterator it;
	it = m_args.find(tmpArgname);
	*/

	map<string, string>::iterator it;
	for(it = m_args.begin(); it != m_args.end(); it++)
	{
		if(strcasecmp(argname.c_str(), it->first.c_str()) == 0)
		{
			break;
		}
	}
	
	if(it == m_args.end())
		return -1;

	argvalue = atoi64(it->second.c_str());

	return 0;
}

int CFormArgs::GetFormArgsFromEnv()
{
	m_args.clear();
	
	char * env;
	env = getenv("REQUEST_METHOD");
	if(env == NULL)
		return -1;
	
	string buf;
	if(strcmp(env, "POST") == 0)
	{
		cin >> buf;
	}
	else if(strcmp(env, "GET") == 0)
	{
		env = getenv("QUERY_STRING");
		if(env == NULL)
			return -1;
		buf = env;
	}
	else
	{
		return -1;
	}
	
	int posStart = 0, posEque, posAnd;
	while(1)
	{
		posEque = buf.find("=", posStart);
		if(posEque == string::npos)
			break;
		
		posAnd = buf.find("&", posEque);
		if(posAnd == string::npos)
		{
			posAnd = buf.size();
		}
		
		string arg = buf.substr(posStart, posAnd-posStart);
		string argname, argvalue;
		int name = 1;
		int i;
		for(i = 0; i < arg.size(); i++)
		{
			if(arg[i] == '=')
			{
				name = 0;
				continue;
			}
			
			char ch;
			if(arg[i] == '+')
				ch = ' ';
			else if(arg[i] == '%' && i < arg.size()-2)
			{
				char str[3];
				unsigned int n;
				str[0] = arg[i+1];
				str[1] = arg[i+2];
				str[2] = '\0';
				sscanf(str, "%x" , &n);
				ch = (unsigned char)n;
				
				i += 2;
			}
			else
				ch = arg[i];
			
			if(name)
				argname += ch;
			else
				argvalue += ch;
		}
		
		//m_args.insert(map<string, string>::value_type(argname, argvalue));
		SetArg(argname, argvalue);
		
		posStart = posAnd + 1; 
	}
	
	return 0;
}

string CFormArgs::GetBoundary()
{
	return m_boundary;
}

string CFormArgs::EncodeVal(string arg)
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
		else if(ch == ' ')
		{
			ret += "+";
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

void CFormArgs::Clear()
{
	m_boundary = "";
	m_args.clear();
}

CHttpRequestMsg::CHttpRequestMsg()
{
	m_cmd = "GET";
	m_version = "HTTP/1.1";
	m_path = "/";
	m_port = 80;
	m_multipart = 0;
	m_useProxy = false;
	SetHeader("Accept", "*/*");
	SetHeader("User-Agent", "Simple Http Client");
	SetHeader("Connection", "Keep-Alive");
}

CHttpRequestMsg::~CHttpRequestMsg()
{
}

int CHttpRequestMsg::SetCmd(const string & cmd)
{
	m_cmd = cmd;

	return 0;
}

int CHttpRequestMsg::SetPath(const string & path)
{
	m_path = path;
	StringTrim(m_path);

	if(m_path.size() == 0)
		m_path = "/";

	if(m_path[0] != '/' && strncasecmp(m_path.c_str(), "http://", 7) != 0)
		m_path = "/" + m_path;

	StringReplace(m_path, "\t", "%20");
	StringReplace(m_path, "\r", "%20");
	StringReplace(m_path, "\n", "%20");
	//string_replace(m_path, "%", "%25");
	StringReplace(m_path, " ", "%20");

	return 0;
}

int CHttpRequestMsg::SetArg(const string & argname, string argvalue)
{
	return m_formArgs.SetArg(argname, argvalue);
}

int CHttpRequestMsg::SetArg(const string & argname, int64 argvalue)
{
	return m_formArgs.SetArg(argname, argvalue);
}

int CHttpRequestMsg::SetProtocol(const string & version)
{
	if(version != "HTTP/1.0" && version != "HTTP/1.1")
		return -1;

	m_version = version;

	return 0;
}

int CHttpRequestMsg::SetHeader(const string & headerName, const string & headerValue)
{
	string tmpname = headerName;
	StringTrim(tmpname);
	if(tmpname.size() == 0)
		return -1;
	if(tmpname.find(":") != string::npos)
		return -1;
	if(tmpname.find(" ") != string::npos)
		return -1;

	string tmpvalue = headerValue;
	StringTrim(tmpvalue);
	StringReplace(tmpvalue, "\t", " ");
	StringReplace(tmpvalue, "\r", " ");
	StringReplace(tmpvalue, "\n", " ");

	map<string, string>::iterator it;
	it = m_headers.find(tmpname);
	if(it != m_headers.end())
		m_headers.erase(it);

	m_headers.insert(map<string, string>::value_type(tmpname, tmpvalue));

	return 0;
}

int CHttpRequestMsg::SetHeader(const string & headerName, int headerValue)
{
	char strInt[30];
	snprintf(strInt, sizeof(strInt), "%d", headerValue);
	return SetHeader(headerName, strInt);
}

int CHttpRequestMsg::SetPostFile(const string & postFileArg, const string & postFn)
{
	m_postFileArg = postFileArg;
	m_postFn = postFn;

	return 0;
}

int CHttpRequestMsg::SetBody(const string & body)
{
	m_body = body;

	return 0;
}

int CHttpRequestMsg::SetHost(const string & host, int port)
{
	m_host = host;
	if(port > 0 && port <= 0xffff)
		m_port = port;
	else
		m_port = 80;

	if(m_port == 80)
		SetHeader("Host", m_host);
	else
		SetHeader("Host", m_host + ":" + itoa64(m_port));

	return 0;
}

int CHttpRequestMsg::SetMultipart(int multipart)
{
	m_multipart = multipart;

	return 0;
}

int CHttpRequestMsg::SetProxy(bool useProxy, const string & auth, const string & user, const string & pass)
{
	m_useProxy = useProxy;
	m_proxyAuth = auth;
	m_proxyUser = user;
	m_proxyPass = pass;

	return 0;
}

int CHttpRequestMsg::GetPath(string & path)
{
	path = m_path;

	return 0;
}

int CHttpRequestMsg::GetFullPath(string & path)
{
	if(m_cmd == "POST")
	{
		path = m_path;
	}
	else
	{
		string formArgsStr;
		m_formArgs.Encode(formArgsStr);
		path = m_path + "?" + formArgsStr;
	}

	return 0;
}

int CHttpRequestMsg::GetArg(const string & argname, string & argvalue)
{
	return m_formArgs.GetArg(argname, argvalue);
}

int CHttpRequestMsg::GetArg(const string & argname, int & argvalue)
{
	return m_formArgs.GetArg(argname, argvalue);
}

int CHttpRequestMsg::GetArg(const string & argname, int64 & argvalue)
{
	return m_formArgs.GetArg(argname, argvalue);
}

int CHttpRequestMsg::GetArgsStr(string & argsStr)
{
	argsStr = m_formArgsStr;
	
	return 0;
}

int CHttpRequestMsg::GetCmd(string & cmd)
{
	cmd = m_cmd;
	
	return 0;
}

int CHttpRequestMsg::GetProtocol(string & version)
{
	version = m_version;

	return 0;
}

int CHttpRequestMsg::GetHeader(const string & headerName, string & headerValue)
{
	map<string, string>::iterator it;
	it = m_headers.find(headerName);
	if(it == m_headers.end())
		return -1;

	headerValue = it->second;

	return 0;
}

int CHttpRequestMsg::GetHeader(int idx, string & headerName, string & headerValue)
{
	if(idx < 0 || idx >= m_headers.size())
		return -1;

	int i;
	map<string, string>::iterator it;
	for(i = 0, it = m_headers.begin(); i <= idx; i++, it++) 
	{
	}

	if(it == m_headers.end())
		return -1;

	headerName = it->first;
	headerValue = it->second;

	return 0;
}

int CHttpRequestMsg::GetBody(string & body)
{
	body = m_body;

	return 0;
}

int CHttpRequestMsg::Encode(string & s)
{
	s = "";

	/*
	if(reqMethod == METHOD_GET)
		m_cmd = "GET";
	else if(reqMethod == METHOD_POST || reqMethod == METHOD_POST_MULTIPART)
		m_cmd = "POST";
	else
		return -1;
	*/

	REQ_METHOD reqMethod;
	if(m_cmd == "GET")
		reqMethod = METHOD_GET;
	else if(m_cmd == "POST" && m_multipart == 0)
		reqMethod = METHOD_POST;
	else if(m_cmd == "POST" && m_multipart != 0)
		reqMethod = METHOD_POST_MULTIPART;
	else
		return -1;

	string formArgsStr;
	m_formArgs.Encode(formArgsStr, reqMethod);

	string path = m_path;
	if(path.size() == 0)
		path = "/";

	if(m_useProxy && m_host.size() > 0)
	{
		if(strcasecmp(path.substr(0, 7).c_str(), "http://") != 0)
		{
			if(m_port == 80)
				path = "http://" + m_host + path;
			else
				path = "http://" + m_host + ":" + itoa64(m_port) + path;
		}

		/*
		if(m_proxyAuth.size() > 0)
			SetHeader("Proxy-Authorization", m_proxyAuth);
		else if(m_proxyUser.size() > 0)
			SetHeader("Proxy-Authorization", "Basic " + Base64Encode(m_proxyUser + ":" + m_proxyPass));
		*/

		if(strnicmp(m_proxyAuth.c_str(), "Basic", 5) == 0)
		{
			if(m_proxyUser.size() > 0)
				SetHeader("Proxy-Authorization", "Basic " + Base64Encode(m_proxyUser + ":" + m_proxyPass));
		}
	}

	if(m_cmd == "GET")
	{
		if(formArgsStr.size() > 0)
		{
			string::size_type pos = path.find("?");
			if(pos != string::npos)
			{
				s = m_cmd + " " + path.substr(0, pos) + "?" + formArgsStr + " " + m_version + "\r\n";
			}
			else
			{
				s = m_cmd + " " + path + "?" + formArgsStr + " " + m_version + "\r\n";
			}
		}
		else
		{
			s = m_cmd + " " + path + " " + m_version + "\r\n";
		}
	}
	else
	{
		s = m_cmd+" "+path+" "+m_version+"\r\n";
	}

	if(m_cmd == "POST" && m_body.size() == 0)
	{
		if(reqMethod == METHOD_POST_MULTIPART)
		{
			SetHeader("Content-Type", "multipart/form-data; boundary=" + m_formArgs.GetBoundary());
		}
		else
		{
			SetHeader("Content-Type", "application/x-www-form-urlencoded");
		}
		SetHeader("Content-Length", formArgsStr.size());
	}
	else if(m_cmd == "POST" && m_body.size() != 0)
	{
		string contentType;
		if(GetHeader("Content-Type", contentType) == -1)
		{
			SetHeader("Content-Type", "application/user-defined");
		}
		SetHeader("Content-Length", m_body.size());
	}

	map<string, string>::iterator it;
	for(it = m_headers.begin(); it != m_headers.end(); it++)
	{
		s += it->first+": "+it->second+"\r\n";
	}
	s += "\r\n";

	if(m_cmd == "POST" && m_body.size() == 0)
	{
		s += formArgsStr;
	}
	else
	{
		s += m_body;
	}

	return 0;
}

int CHttpRequestMsg::Decode(const string & s)
{
	Clear();

	//locate cmd, headers, body 
	string::size_type headersStartPos = s.find("\r\n");
	if(headersStartPos == string::npos)
		return -1;
	headersStartPos += 2;

	string::size_type bodyStartPos = s.find("\r\n\r\n");
	if(bodyStartPos == string::npos)
		return -1;
	bodyStartPos += 4;

	string cmdLine = s.substr(0, headersStartPos-2);
	string headers = s.substr(headersStartPos, bodyStartPos-4-headersStartPos);
	m_body = s.substr(bodyStartPos);

	string::size_type start = 0, end = string::npos;

	//decode cmd
	while(cmdLine[start] == ' ')
		start++;
	end = cmdLine.find(" ", start);
	if(end == string::npos)
		return -1;
	m_cmd = cmdLine.substr(start, end-start);
	if(m_cmd != "GET" && m_cmd != "POST")
		return -1;
	start = end;

	if(m_cmd == "POST")
	{
		m_formArgsStr = m_body;
	}

	//decode path and args
	string tmp_path;
	while(cmdLine[start] == ' ')
		start++;

	end = cmdLine.find(" ", start);
	if(end == string::npos)
		return -1;

	end = cmdLine.find("?", start); //find "?" first 
	if(end == string::npos) //no "?" 
	{
		end = cmdLine.find(" ", start);
		if(end == string::npos)
			return -1;
		tmp_path = cmdLine.substr(start, end-start);
	}
	else
	{
		tmp_path = cmdLine.substr(start, end-start);
		StringTrim(tmp_path);
		start = end;

		start++;
		end = cmdLine.find(" ", start);
		if(end == string::npos)
			return -1;

		if(m_cmd == "GET")
		{
			m_formArgsStr = cmdLine.substr(start, end-start);
			if(m_formArgsStr.find('#') != string::npos)
				m_formArgsStr = m_formArgsStr.substr(0, m_formArgsStr.find('#'));
		}
	}
	start = end;

	//decode path
	m_path = "";
	int j;
	for(j = 0; j < tmp_path.size(); j++)
	{
		if(tmp_path[j] == '%')
		{
			if(j+2 < tmp_path.size())
			{
				unsigned char ch;
				if(tmp_path[j+1] >= '0' && tmp_path[j+1] <= '9')
					ch = (tmp_path[j+1]-'0') * 16;
				else if(tmp_path[j+1] >= 'a' && tmp_path[j+1] <= 'f')
					ch = (tmp_path[j+1]-'a'+10) * 16;
				else if(tmp_path[j+1] >= 'A' && tmp_path[j+1] <= 'F')
					ch = (tmp_path[j+1]-'A'+10) * 16;

				if(tmp_path[j+2] >= '0' && tmp_path[j+2] <= '9')
					ch += (tmp_path[j+2]-'0');
				else if(tmp_path[j+2] >= 'a' && tmp_path[j+2] <= 'f')
					ch += (tmp_path[j+2]-'a'+10);
				else if(tmp_path[j+2] >= 'A' && tmp_path[j+2] <= 'F')
					ch += (tmp_path[j+2]-'A'+10);

				m_path.append(1, ch);

				j += 2;
			}
		}
		else if(tmp_path[j] == '+') 
		{
			m_path.append(1, ' ');
		}
		else
		{
			m_path.append(1, tmp_path[j]);
		}
	}

	//decode version
	while(cmdLine[start] == ' ')
		start++;
	end = cmdLine.find(" ", start);
	if(end == string::npos)
		end = cmdLine.size();

	m_version = cmdLine.substr(start, end-start);
	if(m_version != "HTTP/1.0" && m_version != "HTTP/1.1")
		return -1;
	

	//decode headers
	start = 0;
	while(start < headers.size())
	{
		string header, name, value;

		end = headers.find("\r\n", start);
		if(end == string::npos)
			end = headers.size();
		header = headers.substr(start, end-start);
		start = end+2;

		string::size_type colonPos =  header.find(":");
		if(colonPos == string::npos)
			return -1;

		name = header.substr(0, colonPos);
		value = header.substr(colonPos+1);
		StringTrim(name);
		StringTrim(value);

		m_headers.erase(name);
		m_headers.insert(map<string, string>::value_type(name, value));
	}
	
	string content_type;
	GetHeader("Content-Type", content_type);

	int args_type = -1; //auto
	if(content_type.find("multipart/form-data") != string::npos)
	{
		args_type = 1;
		m_multipart = 1;
	}

	m_formArgs.Decode(m_formArgsStr, m_cmd);

	return 0;
}

int CHttpRequestMsg::SendMsg(int sock, int timeout)
{
	string str;
	Encode(str);

	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	int64 startTime = GetMilSec();
	
	int sentLen = 0;
	while(sentLen < str.size())
	{
		if(timeout > 0 && GetMilSec() - startTime >= timeout)
			return -1;

		int len;
		len = send(sock, str.data()+sentLen, str.size()-sentLen, 0);
		if(len < 0)
		{
			#if (defined(_WIN32) || defined(_WIN64))
			DWORD dw = 0;
			if( (dw=GetLastError()) == 10035)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
			#else
			
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				usleep(10*1000);
				continue;
			}
			else
			{
				return -1;
			}

			#endif
		}
		else if(len == 0) 
		{
			return -1;
		}
		else
		{
			sentLen += len;
		}
	}

	if(sentLen != str.size()) 
	{
		return -1;
	}

	return 0;
}

void SendTo(string s)
{
	printf("%s", s.c_str());
}

int CHttpRequestMsg::PostFile(int sock)
{
	string formArgsStr;
	m_formArgs.Encode(formArgsStr, METHOD_POST_MULTIPART);

	string path = m_path;
	if(path.size() == 0)
		path = "/";


	/*
	if(m_cmd == "POST" && m_body.size() == 0)
	{
		if(reqMethod == METHOD_POST_MULTIPART)
		{
			SetHeader("Content-Type", "multipart/form-data; boundary=" + m_formArgs.GetBoundary());
		}
		else
		{
			SetHeader("Content-Type", "application/x-www-form-urlencoded");
		}
		SetHeader("Content-Length", formArgsStr.size());
	}
	else if(m_cmd == "POST" && m_body.size() != 0)
	{
		string contentType;
		if(GetHeader("Content-Type", contentType) == -1)
		{
			SetHeader("Content-Type", "application/user-defined");
		}
		SetHeader("Content-Length", m_body.size());
	}
	*/

	SetHeader("Content-Type", "multipart/form-data; boundary=" + m_formArgs.GetBoundary());

	formArgsStr = formArgsStr.substr(0, formArgsStr.size()-4) + "\r\n";
	formArgsStr += "Content-Disposition: form-data; name=\"" + m_postFileArg + "\"; filename=\"upload-file\"\r\n";
	formArgsStr += "Content-Type: application/octet-stream\r\n";
	formArgsStr += "\r\n";

	int fileLength = 0;
	FILE * fp = fopen(m_postFn.c_str(), "rb");
	if(fp == NULL)
	{
		printf("Open file failed.\n");
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		fileLength = ftell(fp);
		rewind(fp);
		printf("fileLength: %d\n", fileLength);
	}

	int contentLength = formArgsStr.size() + fileLength + m_formArgs.GetBoundary().size() + 4;
	SetHeader("Content-Length", contentLength);

	string s = "POST "+path+" "+m_version+"\r\n";

	map<string, string>::iterator it;
	for(it = m_headers.begin(); it != m_headers.end(); it++)
	{
		s += it->first+": "+it->second+"\r\n";
	}
	s += "\r\n";
	s += formArgsStr;

	SendTo(s);

	while(!feof(fp))
	{
		char buf[1024] = { 0 };
		int len = fread(buf, 1, sizeof(buf), fp);
		string tmp;
		tmp.assign(buf, len);
		SendTo(tmp);
	}

	fclose(fp);

	//formArgsStr += "hello";
	//formArgsStr += m_formArgs.GetBoundary() + "--\r\n";

	SendTo(m_formArgs.GetBoundary() + "--\r\n");

	

	//cout << s << endl;

	return 0;
}

#if 0
int CHttpRequestMsg::RecvMsg(int sock, int timeout)
{
	Clear();

	string msg;
	int headOK = 0;
	int msgOK = 0;
	int contentLength = 0;
	int recvdContentLength = 0;

	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	int64 startTime = GetMilSec();

	while(1)
	{
		if(timeout > 0 && GetMilSec() - startTime >= timeout)
			return -1;

		char buf[1] = { 0 };
		int len;
		len = recv(sock, buf, 1, 0);
		if(len < 0)
		{
			#if (defined(_WIN32) || defined(_WIN64))
			
			if(GetLastError() == 10035)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
			#else
			
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				usleep(10*1000);
				continue;
			}
			else
			{
				return -1;
			}

			#endif
		}
		else if(len == 0)
		{
			return -1;
		}
		else
		{
			msg.append(1, buf[0]);

			if(msg.size() == 5)
			{
				if(strncmp(msg.c_str(), "GET ", 4) != 0 && strncmp(msg.c_str(), "POST ", 5) != 0	&& strncmp(msg.c_str(), "HEAD ", 5) != 0)
				{
					printf("msg.c_str(): %s\n", msg.c_str()); //debug
					return -1;
				}
			}

			if(headOK)
			{
				recvdContentLength++;
				if(recvdContentLength >= contentLength)
				{
					msgOK = 1;
					break;
				}
			}
		}

		//判断消息头是否接收完成
		if(!headOK && msg.find("\r\n\r\n") != string::npos)
		{
			headOK = 1;

			if(msg.find("Content-Length:") != string::npos)
			{
				int pos = msg.find("Content-Length:") + 15;
				contentLength = atoi(msg.c_str()+pos);
			}
			if(contentLength <= 0)
			{
				msgOK = 1;
				break;
			}
		}
	}

	if(!msgOK)
	{
		return -1;
	}

	if(Decode(msg) != 0) 
	{
		return -1;
	}

	return 0;
}
#endif

#if 1
int CHttpRequestMsg::RecvMsg(int sock, int timeout)
{
	Clear();

	string msg;
	int methodOK = 0;
	int headOK = 0;
	int msgOK = 0;
	int contentLength = 0;
	int recvdContentLength = 0;

	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	int64 startTime = GetMilSec();

	while(1)
	{
		if(timeout > 0 && GetMilSec() - startTime >= timeout)
			return -1;

		const int bufLen = 10*1024;
		char buf[bufLen] = { 0 };
		int len;
		len = recv(sock, buf, bufLen, 0);
		if(len < 0)
		{
			#if (defined(_WIN32) || defined(_WIN64))
			
			if(GetLastError() == 10035)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
			#else
			
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				usleep(10*1000);
				continue;
			}
			else
			{
				return -1;
			}

			#endif
		}
		else if(len == 0)
		{
			return -1;
		}
		else
		{
			msg.append(buf, len);

			if(!methodOK && msg.size() >= 5)
			{
				if(strncmp(msg.c_str(), "GET ", 4) != 0 && strncmp(msg.c_str(), "POST ", 5) != 0	&& strncmp(msg.c_str(), "HEAD ", 5) != 0)
				{
					return -1;
				}

				methodOK = 1;
			}

			if(headOK)
			{
				recvdContentLength += len;
				if(recvdContentLength >= contentLength)
				{
					msgOK = 1;
					break;
				}
			}
		}

		//判断消息头是否接收完成
		if(!headOK && msg.find("\r\n\r\n") != string::npos)
		{
			headOK = 1;

			int headLen = msg.find("\r\n\r\n") + 4;
			recvdContentLength = msg.size() - headLen;

			if(msg.find("Content-Length:") != string::npos)
			{
				int pos = msg.find("Content-Length:") + 15;
				contentLength = atoi(msg.c_str()+pos);
			}
			if(contentLength <= 0 || recvdContentLength >= contentLength)
			{
				msgOK = 1;
				break;
			}
		}
	}

	if(!msgOK)
	{
		return -1;
	}

	if(Decode(msg) != 0) 
	{
		return -1;
	}

	return 0;
}
#endif

string CHttpRequestMsg::Base64Encode(const string & data)
{
	const unsigned char * pData = (unsigned char *)data.data();
	int dataLen = data.size();
	
    //编码表
    const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //返回值
    string strEncode;
    unsigned char Tmp[4]={0};
    int LineLength=0;
    for(int i=0;i<(int)(dataLen / 3);i++)
    {
        Tmp[1] = *pData++;
        Tmp[2] = *pData++;
        Tmp[3] = *pData++;
        strEncode+= EncodeTable[Tmp[1] >> 2];
        strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
        strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
        strEncode+= EncodeTable[Tmp[3] & 0x3F];
        if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
    }
    //对剩余数据进行编码
    int Mod=dataLen % 3;
    if(Mod==1)
    {
        Tmp[1] = *pData++;
        strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
        strEncode+= "==";
    }
    else if(Mod==2)
    {
        Tmp[1] = *pData++;
        Tmp[2] = *pData++;
        strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
        strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
        strEncode+= "=";
    }
    
    return strEncode;
}

int CHttpRequestMsg::Clear()
{
	m_headers.clear();
	m_formArgs.Clear();
	m_formArgsStr = "";
	m_body = "";
	m_host = "";
	m_port = 80;
	m_multipart = 0;
	m_useProxy = false;
	m_proxyAuth = "";
	m_proxyUser = "";
	m_proxyPass = "";

	m_cmd = "GET";
	m_version = "HTTP/1.1";
	m_path = "/";
	SetHeader("Accept", "*/*");
	SetHeader("User-Agent", "Simple Http Client");
	SetHeader("Connection", "Keep-Alive");

	return 0;
}

CHttpResponseMsg::CHttpResponseMsg()
{
	m_version = "HTTP/1.1";
	m_statusCode = "200";
	m_statusDesc = "OK";
	SetHeader("Content-Type", "text/html");
	SetHeader("Access-Control-Allow-Origin", "*");
	SetHeader("Server", "Simple Http Server");
	SetHeader("Connection", "Close");
	m_onlyHead = 0;
	m_headFun = NULL;
	m_headFunArg = NULL;
	m_bodyFun = NULL;
	m_bodyFunArg = NULL;
	m_resHeadFunCalled = 0;
	m_resBodyFunCalled = 0;
	m_resHeadFunRet = 0;
	m_resBodyFunRet = 0;
}

CHttpResponseMsg::~CHttpResponseMsg()
{
}

int CHttpResponseMsg::SetStatus(int code, const string & desc)
{
	if(SetStatusCode(code) == -1)
		return -1;
	if(SetStatusDesc(desc) == -1)
		return -1;

	return 0;
}

int CHttpResponseMsg::SetStatusCode(int code)
{
	if(!(code >= 200 && code <= 999)) {
		return -1;
	}
	
	m_statusCode = StringPrintf("%d", code);

	return 0;
}

int CHttpResponseMsg::SetStatusDesc(const string & desc)
{
	if(desc.size() > 100)
		return -1;

	m_statusDesc = desc;
	StringTrim(m_statusDesc);
	StringReplace(m_statusDesc, "\t", " ");
	StringReplace(m_statusDesc, "\r", " ");
	StringReplace(m_statusDesc, "\n", " ");

	return 0;
}

int CHttpResponseMsg::SetProtocol(const string & version)
{
	if(version != "HTTP/1.0" && version != "HTTP/1.1")
		return -1;

	m_version = version;

	return 0;
}

int CHttpResponseMsg::SetHeader(const string & headerName, const string & headerValue)
{
	string tmpname = headerName;
	StringTrim(tmpname);
	if(tmpname.size() == 0)
		return -1;
	if(tmpname.find(":") != string::npos)
		return -1;
	if(tmpname.find(" ") != string::npos)
		return -1;

	string tmpvalue = headerValue;
	StringTrim(tmpvalue);
	StringReplace(tmpvalue, "\t", " ");
	StringReplace(tmpvalue, "\r", " ");
	StringReplace(tmpvalue, "\n", " ");

	map<string, string>::iterator it;
	it = m_headers.find(tmpname);
	if(it != m_headers.end())
		m_headers.erase(it);

	m_headers.insert(map<string, string>::value_type(tmpname, tmpvalue));

	return 0;
}

int CHttpResponseMsg::SetHeader(const string & headerName, int headerValue)
{
	char strInt[30];
	snprintf(strInt, sizeof(strInt), "%d", headerValue);
	return SetHeader(headerName, strInt);
}

int CHttpResponseMsg::SetBody(const string & body)
{
	m_body = body;
	SetHeader("Content-Length", m_body.size());

	return 0;
}

int CHttpResponseMsg::SetOnlyHead(int onlyHead)
{
	m_onlyHead = onlyHead;

	return 0;
}

int CHttpResponseMsg::SetHeadFun(FPHeadFun fun, void * arg)
{
	m_headFun = fun;
	m_headFunArg = arg;

	return 0;
}

int CHttpResponseMsg::SetBodyFun(FPBodyFun fun, void * arg)
{
	m_bodyFun = fun;
	m_bodyFunArg = arg;

	return 0;
}

int CHttpResponseMsg::AppendBody(const string & body)
{
	m_body += body;
	SetHeader("Content-Length", m_body.size());

	return 0;
}

int CHttpResponseMsg::GetStatus(int & code, string & desc)
{
	code = atoi(m_statusCode.c_str());
	desc = m_statusDesc;

	return 0;
}

int CHttpResponseMsg::GetStatusCode(int & code)
{
	code = atoi(m_statusCode.c_str());

	return 0;
}

int CHttpResponseMsg::GetStatusDesc(string & desc)
{
	desc = m_statusDesc;

	return 0;
}


int CHttpResponseMsg::GetProtocol(string & version)
{
	version = m_version;

	return 0;
}

int CHttpResponseMsg::GetHeader(const string & headerName, string & headerValue)
{
	map<string, string>::iterator it;
	it = m_headers.find(headerName);
	if(it == m_headers.end())
		return -1;

	headerValue = it->second;

	return 0;
}

int CHttpResponseMsg::GetHeader(int idx, string & headerName, string & headerValue)
{
	if(idx < 0 || idx >= m_headers.size())
		return -1;

	int i;
	map<string, string>::iterator it;
	for(i = 0, it = m_headers.begin(); i <= idx; i++, it++) {
	}

	if(it == m_headers.end())
		return -1;

	headerName = it->first;
	headerValue = it->second;

	return 0;
}

int CHttpResponseMsg::GetHead(string & head)
{
	head = m_head;

	return 0;
}

int CHttpResponseMsg::GetBody(string & body)
{
	body = m_body;

	return 0;
}

int CHttpResponseMsg::GetResHeadFunRet(int & ret)
{
	if(!m_resHeadFunCalled)
		return -1;
	
	ret = m_resHeadFunRet;

	return 0;
}

int CHttpResponseMsg::GetResBodyFunRet(int & ret)
{
	if(!m_resBodyFunCalled)
		return -1;
	
	ret = m_resBodyFunRet;

	return 0;
}

int CHttpResponseMsg::Encode(string & s)
{
	s = "";

	if(m_version != "HTTP/1.0" && m_version != "HTTP/1.1")
		return -1;
	if(m_statusCode.size() != 3)
		return -1;
	if(!(isdigit(m_statusCode[0]) && isdigit(m_statusCode[1]) && isdigit(m_statusCode[2])))
		return -1;

	s = m_version+" "+m_statusCode+" "+m_statusDesc+"\r\n";

	string tmp;
	if(GetHeader("Content-Length", tmp) != 0)
	{
		SetHeader("Content-Length", m_body.size());
	}
	
	map<string, string>::iterator it;
	for(it = m_headers.begin(); it != m_headers.end(); it++)
	{
		s += it->first+": "+it->second+"\r\n";
	}
	s += "\r\n";

	s += m_body;

	return 0;
}


int CHttpResponseMsg::Decode(const string & s)
{
	Clear();

	//locate cmd, headers, body 
	string::size_type headersStartPos = s.find("\r\n");
	if(headersStartPos == string::npos)
		return -1;
	headersStartPos += 2;

	string::size_type bodyStartPos = s.find("\r\n\r\n");
	if(bodyStartPos == string::npos)
		return -1;
	bodyStartPos += 4;

	string cmdLine = s.substr(0, headersStartPos-2);
	string headers = s.substr(headersStartPos, bodyStartPos-4-headersStartPos);
	m_body = s.substr(bodyStartPos);

	string::size_type start = 0, end = string::npos;

	//decode cmd
	while(cmdLine[start] == ' ')
		start++;
	end = cmdLine.find(" ", start);
	if(end == string::npos)
		return -1;
	m_version = cmdLine.substr(start, end-start);
	if(m_version != "HTTP/1.0" && m_version != "HTTP/1.1")
		return -1;
	start = end;

	while(cmdLine[start] == ' ')
		start++;
	end = cmdLine.find(" ", start);
	if(end == string::npos)
		return -1;
	m_statusCode = cmdLine.substr(start, end-start);
	if(m_statusCode.size() != 3 || (atoi(m_statusCode.c_str()) < 200 || atoi(m_statusCode.c_str()) > 600))
		return -1;

	start = end;
	while(cmdLine[start] == ' ')
		start++;
	if(start < cmdLine.size())
		m_statusDesc = cmdLine.substr(start);

	//decode headers
	start = 0;
	while(start < headers.size())
	{
		string header, name, value;

		end = headers.find("\r\n", start);
		if(end == string::npos)
			end = headers.size();
		header = headers.substr(start, end-start);
		start = end+2;

		string::size_type colonPos =  header.find(":");
		if(colonPos == string::npos)
			return -1;

		name = header.substr(0, colonPos);
		value = header.substr(colonPos+1);
		StringTrim(name);
		StringTrim(value);

		m_headers.erase(name);
		m_headers.insert(map<string, string>::value_type(name, value));
	}

	return 0;
}


int CHttpResponseMsg::SendMsg(int sock, int timeout)
{
	string str;
	Encode(str);

	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	int64 startTime = GetMilSec();
	
	int sentLen = 0;
	while(sentLen < str.size())
	{
		if(timeout > 0 && GetMilSec() - startTime >= timeout)
			return -1;

		int len;
		len = send(sock, str.data()+sentLen, str.size()-sentLen, 0);
		if(len < 0)
		{
			#if (defined(_WIN32) || defined(_WIN64))
			
			if(GetLastError() == 10035)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
			#else
			
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				usleep(10*1000);
				continue;
			}
			else
			{
				return -1;
			}

			#endif
		}
		else if(len == 0) 
		{
			return -1;
		}
		else
		{
			sentLen += len;
		}
	}

	if(sentLen != str.size()) 
	{
		return -1;
	}

	return 0;
}

#if 0
int CHttpResponseMsg::RecvMsg(int sock, int timeout)
{
	Clear();

	string msg;
	int headOK = 0;
	int msgOK = 0;
	int contentLength = 0;
	int recvdContentLength = 0;

	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	int64 startTime = GetMilSec();

	while(1)
	{
		if(timeout > 0 && GetMilSec() - startTime >= timeout)
			return -1;

		char buf[1] = { 0 };
		int len;
		len = recv(sock, buf, 1, 0);
		if(len < 0)
		{
			#if (defined(_WIN32) || defined(_WIN64))
			
			if(GetLastError() == 10035)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
			#else
			
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				usleep(10*1000);
				continue;
			}
			else
			{
				return -1;
			}

			#endif
		}
		else if(len == 0)
		{
			return -1;
		}
		else
		{
			msg.append(1, buf[0]);

			if(headOK)
			{
				recvdContentLength++;
				if(recvdContentLength >= contentLength)
				{
					msgOK = 1;
					break;
				}
			}
		}

		//判断消息头是否接收完成
		if(!headOK && msg.find("\r\n\r\n") != string::npos)
		{
			headOK = 1;

			if(msg.find("Content-Length:") != string::npos)
			{
				int pos = msg.find("Content-Length:") + 15;
				contentLength = atoi(msg.c_str()+pos);
			}
			if(contentLength <= 0)
			{
				msgOK = 1;
				break;
			}
		}
	}

	if(!msgOK)
	{
		return -1;
	}

	if(Decode(msg) != 0) 
	{
		return -1;
	}

	return 0;
}
#endif

int CHttpResponseMsg::RecvMsg(int sock, int timeout)
{
	Clear();

	string msg;
	int headOK = 0;
	int msgOK = 0;
	int methodOK = 0;
	int fileLength = -1;
	int contentLength = 0;
	int recvdContentLength = 0;
	string transferEncoding;
	int curChunkLength = -1;
	int curChunkReadLength = 0;
	string stateCode = "200";
	int contentOffset = 0;
	string contentType;
	string contentBuf;

	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	int64 startTime = GetMilSec();

	while(true)
	{
		if(timeout > 0 && GetMilSec() - startTime >= timeout)
			return -1;

		const int bufLen = 10*1024;
		char buf[bufLen] = { 0 };
		int len;
		if(!headOK)
			len = 1;
		else
			len = bufLen;

		len = recv(sock, buf, len, 0);
		if(len < 0)
		{
			#if (defined(_WIN32) || defined(_WIN64))
			
			if(GetLastError() == 10035)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
			#else
			
			if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
			{
				usleep(10*1000);
				continue;
			}
			else
			{
				return -1;
			}

			#endif
		}
		else if(len == 0)
		{
			return -1;
		}
		else
		{
			if(headOK && m_bodyFun != NULL && (stateCode == "200" || stateCode == "206"))
			{
				contentBuf.append(buf, len);
				if(contentBuf.size() >= 1024*1024)
				{
					m_resBodyFunCalled = 1;
					m_resBodyFunRet = m_bodyFun(m_bodyFunArg, contentType.c_str(), fileLength, contentOffset, contentBuf.data(), contentBuf.size());
					if(m_resBodyFunRet != 0)
					{
						return -1;
					}
					contentOffset += contentBuf.size();
					contentBuf.clear();
				}
			}
			else
			{
				msg.append(buf, len);

				/*
				if(transferEncoding != "chunked")
				{
					msg.append(buf, len);
				}
				else
				{
					if(curChunkLength == -1 || (curChunkLength > 0 && curChunkReadLength >= curChunkLength))
					{
						curChunkLength = strtol(buf, NULL, 16);
						if(curChunkLength <= 0)
						{
							msgOK = 1;
							break;
						}

						if(strstr(buf, "\r\n") == NULL)
						{
							return -1;
						}

						int offset = strstr(buf, "\r\n") - buf + 2;
						msg.append(buf + offset, len - offset);
						curChunkReadLength = 0;
					}
				}
				*/
			}

			if(!methodOK && msg.size() >= 5)
			{
				if(strncmp(msg.c_str(), "HTTP/", 5) != 0)
				{
					return -1;
				}

				methodOK = 1;
			}

			if(headOK)
			{
				recvdContentLength += len;
				if(transferEncoding == "chunked")
				{
					string body = msg.substr(m_head.size());
					string decodedBody;
					if(DecodeChunk(body, decodedBody) == 0)
					{
						msg = m_head + decodedBody;
						msgOK = 1;
						break;
					}
				}
				else if(recvdContentLength >= contentLength)
				{
					msgOK = 1;
					if(contentBuf.size() > 0)
					{
						m_resBodyFunCalled = 1;
						m_resBodyFunRet = m_bodyFun(m_bodyFunArg, contentType.c_str(), fileLength, contentOffset, contentBuf.data(), contentBuf.size());
						if(m_resBodyFunRet != 0)
						{
							return -1;
						}
						contentOffset += contentBuf.size();
						contentBuf.clear();
					}
					break;
				}
			}
		}

		//判断消息头是否接收完成
		if(!headOK && msg.find("\r\n\r\n") != string::npos)
		{
			headOK = 1;
			m_head = msg.substr(0, msg.find("\r\n\r\n")+4);

			if(m_headFun != NULL)
			{
				m_resHeadFunCalled = 1;
				m_resHeadFunRet = m_headFun(m_headFunArg, m_head.data(), m_head.size());
				if(m_resHeadFunRet != 0)
				{
					return -1;
				}
			}

			GetHTTPStatusCode(msg, stateCode);

			string range;
			if(GetHTTPHeader(msg, "Content-Range", range) == 0)
			{
				StringLower(range);
				int ret = sscanf(range.c_str(), "bytes %d-", &contentOffset);
				if(ret != 1)
				{
					contentOffset = 0;
				}

				if(range.find("/") != string::npos)
				{
					fileLength = atoi(range.substr(range.find("/")+1).c_str());
				}
			}

			if(m_onlyHead)
			{
				//msg = msg.substr(0, msg.find("\r\n\r\n"));
				msgOK = 1;
				break;
			}

			if(GetHTTPHeader(msg, "Content-Type", contentType) == 0)
			{
				//contentType = "application/octet-stream";
			}

			string strContentLength;
			if(GetHTTPHeader(msg, "Content-Length", strContentLength) == 0)
			{
				contentLength = atoi(strContentLength.c_str());
				if(fileLength == -1)
				{
					fileLength = contentLength;
				}
			}
			else if(GetHTTPHeader(msg, "Transfer-Encoding", transferEncoding) == 0)
			{
				StringLower(transferEncoding);
			}

			if(transferEncoding != "chunked" && contentLength <= 0)
			{
				msgOK = 1;
				break;
			}
		}
	}

	if(!msgOK)
	{
		return -1;
	}

	if(Decode(msg) != 0) 
	{
		return -1;
	}

	return 0;
}

int CHttpResponseMsg::Clear()
{
	m_headers.clear();
	m_body = "";

	m_version = "";
	m_statusCode = "";
	m_statusDesc = "";

	m_resHeadFunCalled = 0;
	m_resBodyFunCalled = 0;
	m_resHeadFunRet = 0;
	m_resBodyFunRet = 0;

	return 0;
}
