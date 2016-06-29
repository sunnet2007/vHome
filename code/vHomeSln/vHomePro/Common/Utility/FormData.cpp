#include "FormData.h"
#include <sstream>

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

CFormData::CFormData()
{
}

int CFormData::Dump(string & str)
{
	map<string, string>::iterator it;
	for(it = m_args.begin(); it != m_args.end(); it++)
	{
		str += it->first + " = " + it->second;
		str += "\n";
	}

	return 0;
}

int CFormData::Encode(string & str, int type)
{
	str = "";
	
	if(m_args.size() == 0)
		return 0;

	if(type == FORM_DATA_MULTIPART) 
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

int CFormData::Decode(const string & args, int type)
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
							unsigned char ch;
							if(arg_val[1][j+1] >= '0' && arg_val[1][j+1] <= '9')
								ch = (arg_val[1][j+1]-'0') * 16;
							else if(arg_val[1][j+1] >= 'a' && arg_val[1][j+1] <= 'f')
								ch = (arg_val[1][j+1]-'a'+10) * 16;
							else if(arg_val[1][j+1] >= 'A' && arg_val[1][j+1] <= 'F')
								ch = (arg_val[1][j+1]-'A'+10) * 16;

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

int CFormData::Decode(const char * buf, int len, int type)
{
	string args(buf, len);
	return Decode(args, type);
}

int CFormData::SetArg(const string & argname, string argvalue)
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

int CFormData::GetArg(const string & argname, string & argvalue)
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

int CFormData::GetArg(const string & argname, int & argvalue)
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

int CFormData::SetArg(const string & argname, int64 argvalue)
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

int CFormData::GetArg(const string & argname, int64 & argvalue)
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

int CFormData::GetFormArgsFromEnv()
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

string CFormData::GetBoundary()
{
	return m_boundary;
}

string CFormData::EncodeVal(string arg)
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

void CFormData::Clear()
{
	m_boundary = "";
	m_args.clear();
}
