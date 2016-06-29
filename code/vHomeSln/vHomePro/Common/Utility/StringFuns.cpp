
#include "StringFuns.h"

/*
int string_find_nocase(const string & str, const string & substr, int start_pos)
{
	if(start_pos < 0 || start_pos >= str.size() || substr.size() == 0)
		return -1;

	char *  pPos;
	pPos = strcasestr(str.c_str()+start_pos, substr.c_str());
	if(pPos == NULL)
	{
		return -1;
	}
	else
	{
		return pPos-str.c_str();
	}
}
*/

int StringReplace(string & str, const string & old_str, const string & new_str)
{
	if(old_str.size() == 0)
		return 0;

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

/*
int string_replace_nocase(string & str, const string & old_str, const string & new_str)
{
	string::size_type pos = 0;
	string::size_type oldlen = old_str.size();
	string::size_type newlen = new_str.size();
	int n = 0;
	//while((pos=s.find(old_str, pos)) != string::npos)
	while((pos=string_find_nocase(str, old_str, pos)) != string::npos)
	{
		str.replace(pos, oldlen, new_str);
		pos += newlen;
		n++;
	}

	return n;
}
*/

int string_compare_nocase(const string & str1, const string & str2)
{
	return strcasecmp(str1.c_str(), str2.c_str());
}

int StringLower(string & str)
{
	int i;
	for(i = 0; i < str.size(); i++)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}

	return 0;
}

int StringUpper(string & str)
{
	int i;
	for(i = 0; i < str.size(); i++)
	{
		if(str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
	}

	return 0;
}

string::size_type StringFindNoCase(string str, string substr, string::size_type start_pos)
{
	if(start_pos == string::npos || start_pos >= str.size() || substr.size() == 0)
		return string::npos;

	StringLower(str);
	StringLower(substr);

	const char *  pPos;
	pPos = strstr(str.c_str()+start_pos, substr.c_str());
	if(pPos == NULL)
	{
		return string::npos;
	}
	else
	{
		return pPos-str.c_str();
	}
}

/*
int StringUpper(string & s) {
	transform(s.begin(), s.end(), s.begin(), toupper);

	return 0;
}

int string_lower(string & s) {
	transform(s.begin(),s.end(),s.begin(),tolower);

	return 0;
}
*/

void StringTrim(string & text) {
	int i = 0, j = text.size()-1;
	while(i <= j) {
		if(text[i] == ' ' || text[i] == '\t' || text[i] == '\r' || text[i] == '\n') {
			i++;
			continue;
		}
		if(text[j] == ' ' || text[j] == '\t' || text[j] == '\r' || text[j] == '\n') {
			j--;
			continue;
		}

		break;
	}

	if(i == 0 && j == text.size()-1) {
		return;
	}

	if(i <= j) {
		text = text.substr(i, j-i+1);
	}
	else {
		text = "";
	}
}

string StringPrintf(const char * format, ...)
{
	if(format[0] == '\0')
		return string("");

	char s[1024 * 10] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(s, sizeof(s), format, args);
    va_end(args);

	return string(s);
}

int StringSplit(string str, vector<string> & arr, char delim)
{
	string strDelim;
	strDelim.assign(1, delim);
	return StringSplit(str, arr, strDelim);
}

int StringSplit(string str, vector<string> & arr, string delim)
{
	arr.clear();
	if(delim.size() == 0)
	{
		arr.push_back(str);
		return 0;
	}

	while(true)
	{
		string::size_type p = str.find(delim);
		if(p != string::npos)
		{
			arr.push_back(str.substr(0, p));
			str = str.substr(p+delim.size());
		}
		else
		{
			arr.push_back(str);
			break;
		}
	}

	return 0;
}

int str_hex(const void * org, int org_len, void * dst) {
	unsigned char * o = (unsigned char *)org;
	char * d = (char *)dst;

	int i;
	for(i = 0; i < org_len; i++) {
		sprintf(d+i*2, "%02X", o[i]);
	}

	return 0;
}

int str_unhex(const void * org, int org_len, void * dst) {
	char * o = (char *)org;
	unsigned char * d = (unsigned char *)dst;

	int i;
	for(i = 0; i < org_len/2; i++) {
		char hs[3];
		hs[0] = o[i*2];
		hs[1] = o[i*2+1];
		hs[2] = '\0';
		if(!((hs[0] >= '0' && hs[0] <= '9') || (hs[0] >= 'a' && hs[0] <= 'f') || (hs[0] >= 'A' && hs[0] <= 'F')))
			return -1;
		if(!((hs[1] >= '0' && hs[1] <= '9') || (hs[1] >= 'a' && hs[1] <= 'f') || (hs[1] >= 'A' && hs[1] <= 'F')))
			return -1;

		//unsigned char h;
		//unsigned int h;
		//sscanf(hs, "%X", &h);

		d[i] = strtol(hs, NULL, 16);
	}

	return 0;
}

void WCharToChar(const wchar_t * wchar, char * chr, int length)  
{
#if (defined(_WIN32) || defined(_WIN64))
	WideCharToMultiByte( CP_ACP, 0, wchar, -1, chr, length, NULL, NULL );
#endif
}

//通配符匹配
//返回1匹配成功，返回0匹配失败
int StringWildcardMatch(const string & format, const string & str)
{  
	int formatLen = format.size();
	int strLen = str.size();

	if(formatLen == 0 && strLen == 0)
		return 1;

	//实际使用时根据strl的长度来动态分配表的内存
	char matchmap[128][128];
	memset(matchmap, 0, 128*128); 
	matchmap[0][0] = 1;  
	int i, j, k;  
	//遍历目标字符串符串
	for(i = 1; i<= strLen; ++i)  
	{  
		//遍历通配符串
		for(j = 1; j<=formatLen; ++j)
		{
			//当前字符之前的字符是否已经得到匹配
			if(matchmap[i-1][j-1])
			{
				//匹配当前字符
				if(str[i-1] == format[j-1] || format[j-1] == '?')
				{ 
					matchmap[i][j] = 1; 
					//考虑星号在末尾的情况
					if( i  == strLen && j < formatLen)
					{
						for ( k = j+1 ; k <= formatLen ; ++k )
						{
							if( '*' == format[k-1])
							{
								matchmap[i][k] = 1;
							}
							else
							{
								break;
							}
						}
					}
				}
				else if(format[j-1] == '*')
				{
					//遇到星号，目标字符串到末尾都能得到匹配
					for(k = i-1; k<=strLen; ++k)
					{
						matchmap[k][j] = 1;  
					}
				}
			}
		}
		//如果当前字符得到了匹配则继续循环，否则匹配失败
		for(k = 1; k<=formatLen; ++k)  
		{
			if(matchmap[i][k])
			{
				break; 
			}
		}
		if(k>formatLen)
		{
			return 0;  
		}
	}

	return (int) matchmap[strLen][formatLen];  
}  


#if (defined(_WIN32) || defined(_WIN64))

static char * stristr(const char *String, const char *Pattern)
{
      char *pptr, *sptr, *start;
      unsigned int  slen, plen;

      for (start = (char *)String,
           pptr  = (char *)Pattern,
           slen  = strlen(String),
           plen  = strlen(Pattern);

           /* while string length not shorter than pattern length */

           slen >= plen;

           start++, slen--)
      {
            /* find start of pattern in string */
            while (toupper(*start) != toupper(*Pattern))
            {
                  start++;
                  slen--;

                  /* if pattern longer than string */

                  if (slen < plen)
                        return(NULL);
            }

            sptr = start;
            pptr = (char *)Pattern;

            while (toupper(*sptr) == toupper(*pptr))
            {
                  sptr++;
                  pptr++;

                  /* if end of pattern then pattern was found */

                  if ('\0' == *pptr)
                        return (start);
            }
      }
      return(NULL);
}

#define strcasestr stristr

#endif

#define IS_BLANK_CH(ch) (ch == ' '|| ch == '\t' || ch == '\r' || ch == '\n')

//功能:
//对指定的html语句，能把修饰符，script, style全部去掉，并删多余空白字符。
//对于空白字符，按照IE的显示规则，即多个空白显示为一个空白。
//对于多行<br>,<p>，则只保留一行。
//对于中文空格，不进行任何转换，原样保留。
//对于转意符&nbsp;等，则不进行转意，原样保留。

string ExtractHtmlText(const string & src, int startPos)
{
	string result;
	if(src.size() < 10) 
		return "";

	if(startPos < 0 || startPos >= src.size())
		return "";
	
	const char * htmlStr;
	int htmlStrLen;
	
	htmlStr = src.c_str()+startPos;
	htmlStrLen = src.size()-startPos;
	
	char * text = new char[htmlStrLen+1];
	if(text == NULL)
		return "";
	
	int i = 0;
	int j = 0;
	int textmode = 1;
	char tagname[100];
	int tagnamelen = 0;
	int tagstart = 0;
	while(i < htmlStrLen)
	{
		if(htmlStr[i] == '\r') //ignore any '\r'
		{
			i++;
			continue;
		}

		if(htmlStr[i] == '<')
		{
			if(strncmp(htmlStr+i, "<!--", 4) == 0) //comment start
			{
				const char * p = strstr(htmlStr+i+4, "-->");
				if(p != NULL) //comment end
				{
					i = (int)(p-htmlStr+3);
					continue;
				}
				else //line is comment
				{
					p = strstr(htmlStr+i+4, "\n");
					if(p != NULL)
					{
						i = (int)(p-htmlStr)+1;
						continue;
					}
					else
					{
						break;
					}
				}
			}

			textmode = 0;
			strcpy(tagname, "");
			tagnamelen = 0;
			tagstart = 0;
			i++;
			continue;
		}
		if(htmlStr[i] == '>' && textmode == 0)
		{
			textmode = 1;
			if(strcasecmp(tagname, "br") == 0)
			{
				if(j > 0)
				{
					if(text[j-1] != '\n')
					{
						text[j++] = '\n';
					}
				}
			}
			else if(strcasecmp(tagname, "p") == 0)
			{
				if(j > 0)
				{
					if(text[j-1] != '\n')
					{
						text[j++] = '\n';
						//text[j++] = '\n'; //noly leave one blank line
					}
				}
			}
			else if(strcasecmp(tagname, "script") == 0) //script start
			{
				//to find the script end tag

				const char * p = strcasestr(htmlStr+i+1, "</script>");
				if(p != NULL)
				{
					i = (int)(p-htmlStr+9);
					strcpy(tagname, "");
					tagnamelen = 0;
					tagstart = 0;
					
					continue;
				}

				p = strstr(htmlStr+i+1, "<");
				if(p == NULL) // all left is script
				{
					break;
				}
				
				p = strstr(p+1, "/");
				if(p == NULL) // all left is script
				{
					break;
				}
				
				p = strcasestr(p+1, "script");
				if(p == NULL) // all left is script
				{
					break;
				}

				p = strcasestr(p+6, ">");
				if(p == NULL) // all left is script
				{
					break;
				}

				i = (int)(p-htmlStr)+1;
				strcpy(tagname, "");
				tagnamelen = 0;
				tagstart = 0;
				
				continue;
			}
			else if(strcasecmp(tagname, "style") == 0) //style start
			{
				//to find the style end tag

				const char * p = strcasestr(htmlStr+i+1, "</style>");
				if(p != NULL)
				{
					i = (int)(p-htmlStr+8);
					strcpy(tagname, "");
					tagnamelen = 0;
					tagstart = 0;
					
					continue;
				}

				p = strstr(htmlStr+i+1, "<");
				if(p == NULL) // all left is style
				{
					break;
				}
				
				p = strstr(p+1, "/");
				if(p == NULL) // all left is style
				{
					break;
				}
				
				p = strcasestr(p+1, "style");
				if(p == NULL) // all left is style
				{
					break;
				}

				p = strcasestr(p+5, ">");
				if(p == NULL) // all left is style
				{
					break;
				}

				i = (int)(p-htmlStr)+1;
				strcpy(tagname, "");
				tagnamelen = 0;
				tagstart = 0;
				
				continue;
			}

			strcpy(tagname, "");
			tagnamelen = 0;
			tagstart = 0;
			
			i++;
			continue;
		}

		if(textmode)
		{
			//chinese space char 0xA1A1
			/*if((unsigned char)htmlStr[i] == 0xA1 && (unsigned char)htmlStr[i+1] == 0xA1)
			{
				text[j++] = ' ';
				i++; //note must i++
			}
			else */
			if(IS_BLANK_CH(htmlStr[i]))
			{
				//if j == 0 the any blank ch will be filtered.
				if(j > 0)
				{
					if(!IS_BLANK_CH(text[j-1]))
					{
						//in html any multi space char '\n', '\t', ' ' will trated as only one ' '
						//text[j++] = htmlStr[i];
						text[j++] = ' ';
					}
				}
			}
			else
			{
				text[j++] = htmlStr[i];
			}
		}
		else
		{
			if(tagstart == 0)
			{
				if(!IS_BLANK_CH(htmlStr[i]))
					tagstart = 1;
			}
			else if(tagstart == 1)
			{
				if(IS_BLANK_CH(htmlStr[i])
					|| htmlStr[i] == '>' || htmlStr[i] == '\"' || htmlStr[i] == '\'')
					tagstart = 2;
			}

			if(tagstart == 1 && tagnamelen < 100-1)
			{
				tagname[tagnamelen] = htmlStr[i];
				tagname[tagnamelen+1] = '\0';
				tagnamelen++;
			}
		}

		/*if(j == maxTextLen)
		{
			text[j] = '\0';
			return 1;
		}*/

		i++;
	}
	text[j] = '\0';
	
	j--;
	while(j > 0 && IS_BLANK_CH(text[j])) 
	{
		text[j] = '\0';
		j--;
	}

	result = text;

	delete[] text;
	
	return result;
}
