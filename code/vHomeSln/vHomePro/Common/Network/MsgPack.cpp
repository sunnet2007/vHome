#include "stdafx.h"
#include "MsgPack.h"

//#define int64 __int64

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

CMsgPack::CMsgPack() {
}

CMsgPack::~CMsgPack() {
}

int CMsgPack::SetName(string name) {
	m_name = name;

	return 0;
}

int CMsgPack::GetName(string & name) {
	name = m_name;

	return 0;
}

int CMsgPack::SetArg(string name, int val) {
	if(name.size() == 0 || name.find(":") != string::npos) {
		return -1;
	}

	StringUpper(name);

	if(m_data.find(name) != m_data.end()) {
		m_data.erase(name);
	}

	char tmp[50];
	snprintf(tmp, sizeof(tmp), "%d", val);
	m_data.insert(map<string, string>::value_type(name, tmp));

	return 0;
}

int CMsgPack::SetArg(string name, int64 val) {
	if(name.size() == 0 || name.find(":") != string::npos) {
		return -1;
	}

	StringUpper(name);

	if(m_data.find(name) != m_data.end()) {
		m_data.erase(name);
	}

	char tmp[50];
	#if (defined(_WIN32) || defined(_WIN64))
	snprintf(tmp, sizeof(tmp), "%I64d", val);
	#else
	snprintf(tmp, sizeof(tmp), "%lld", val);
	#endif
	m_data.insert(map<string, string>::value_type(name, tmp));

	return 0;
}

int CMsgPack::SetArg(string name, const string & val) {
	if(name.size() == 0 || name.find(":") != string::npos) {
		return -1;
	}

	StringUpper(name);

	if(m_data.find(name) != m_data.end()) {
		m_data.erase(name);
	}

	m_data.insert(map<string, string>::value_type(name, val));

	return 0;
}

int CMsgPack::SetArg(string name, const char * val) {
	if(name.size() == 0 || name.find(":") != string::npos) {
		return -1;
	}

	StringUpper(name);

	if(m_data.find(name) != m_data.end()) {
		m_data.erase(name);
	}

	if(val == NULL) {
		m_data.insert(map<string, string>::value_type(name, ""));
	}
	else {
		m_data.insert(map<string, string>::value_type(name, val));
	}

	return 0;
}

int CMsgPack::SetArg(string name, const void * val, int byte_num) {
	if(name.size() == 0 || name.find(":") != string::npos) {
		return -1;
	}

	StringUpper(name);

	if(m_data.find(name) != m_data.end()) {
		m_data.erase(name);
	}

	if(val == NULL || byte_num <= 0) {
		m_data.insert(map<string, string>::value_type(name, ""));
	}
	else {
		string s_val;
		s_val.assign((const char*)val, byte_num);
		m_data.insert(map<string, string>::value_type(name, s_val));
	}

	return 0;
}

int CMsgPack::GetArg(string name, int & val) {
	StringUpper(name);

	if(m_data.find(name) != m_data.end()) {
		val = atoi(m_data.find(name)->second.c_str());
		return 0;
	}
	else {
		return -1;
	}
}

int CMsgPack::GetArg(string name, int64 & val) {
	StringUpper(name);

	if(m_data.find(name) != m_data.end()) {
		//val = atoi(m_data.find(name)->second.c_str());
		#if (defined(_WIN32) || defined(_WIN64))
		sscanf(m_data.find(name)->second.c_str(), "%I64d", &val);
		#else
		sscanf(m_data.find(name)->second.c_str(), "%lld", &val);
		#endif
		return 0;
	}
	else {
		return -1;
	}
}

int CMsgPack::GetArg(string name, string & val) {
	StringUpper(name);

	if(m_data.find(name) != m_data.end()) {
		val = m_data.find(name)->second;
		return 0;
	}
	else {
		return -1;
	}
}

int CMsgPack::GetArg(string name, void * val, int * byte_num) {
	if(val == NULL || byte_num == NULL)
		return -1;

	StringUpper(name);

	map<string, string>::iterator it = m_data.find(name);
	if(it != m_data.end()) {
		if(*byte_num < it->second.size()) {
			return -2;
		}
		*byte_num = it->second.size();

		memcpy(val, it->second.data(), it->second.size());

		return 0;
	}
	else {
		return -1;
	}
}

int CMsgPack::Encode(string & str) {
	string boundary = GenBoundary();
	str = "$MSG_PACK$";
	str += "----------";// body length
	str += "BOUNDARY:" +  boundary;
	str += "NAME:" + m_name + boundary;

	map<string, string>::iterator it;
	for(it = m_data.begin(); it != m_data.end(); it++) {
		str += it->first + ":" + it->second + boundary;
	}

	char tmp[50];
	snprintf(tmp, sizeof(tmp), "%-10d", str.size()-20);
	str.replace(10, 10, tmp);

	return 0;
}

int CMsgPack::Decode(string & str) {
	Clear();

	if(strncmp(str.c_str(), "$MSG_PACK$", 10) != 0)
		return -1;

	string slen = str.substr(10, 10);
	if(slen.size() != 10)
		return -1;

	int len = atoi(slen.c_str());
	if(str.size()-20 < len)
		return -1;

	if(strncmp(str.c_str()+10+10, "BOUNDARY:", 9) != 0)
		return -1;

	string boundary = str.substr(10+10+9, 12);
	if(boundary.size() != 12)
		return -1;
	
	if(strncmp(str.c_str()+10+10+9+12, "NAME:", 5) != 0)
		return -1;

	string::size_type pos1 = 10+10+9+12+5;
	string::size_type pos2;
	if((pos2 = str.find(boundary, pos1)) == string::npos)
		return -1;
	m_name = str.substr(pos1, pos2-pos1);

	pos1 = pos2 + 12;
	while(1) {
		if((pos2 = str.find(":", pos1)) == string::npos)
			break;
		string name = str.substr(pos1, pos2-pos1);
		pos1 = pos2 + 1;

		if((pos2 = str.find(boundary, pos1)) == string::npos)
			return -1;
		string val = str.substr(pos1, pos2-pos1);

		m_data.insert(map<string, string>::value_type(name, val));
		pos1 = pos2 + 12;
		if(pos1 >= len+20)
			break;
	}

	return 0;
}

int CMsgPack::SendMsg(int sock, int timeout) {
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
	while(sentLen < str.size()) {
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

		sentLen += len;
	}
	
	if(sentLen != str.size()) {
		return -1;
	}

	return 0;
}

int CMsgPack::RecvMsg(int sock, int timeout) {
	string head;
	string body;
	int head_ok = 0;
	int msg_ok = 0;
	int content_length = 0;
	int recvd_content_length = 0;


	#if (defined(_WIN32) || defined(_WIN64))
	unsigned long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);
	#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif

	int64 startTime = GetMilSec();

	const int buf_len = 100*1024;
	char buf[buf_len] = { 0 };

	while(1) {
		if(timeout > 0 && GetMilSec() - startTime >= timeout)
			return -1;

		int len;
		if(!head_ok) {
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
			
			head.append(1, buf[0]);
		}
		else { //recv body
			if(content_length - recvd_content_length < buf_len) {
				len = content_length - recvd_content_length;
			}
			else {
				len = buf_len;
			}
			
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

			string b;
			b.assign(buf, len);
			body += b;

			recvd_content_length += len;
			if(recvd_content_length >= content_length) {
				msg_ok = 1;
				break;
			}
		}

		//判断消息头是否接收完成
		if(!head_ok && head.size() == 20) {
			head_ok = 1;

			if(strncmp(head.c_str(), "$MSG_PACK$", 10) != 0) {
				return -1;
			}

			content_length = atoi(head.c_str()+10);
			if(content_length <= 0) {
				msg_ok = 1;
				break;
			}
		}
	}

	if(!msg_ok) {
		return -1;
	}

	string msg = head + body;
	if(Decode(msg) == -1) {
		return -1;
	}
	
	return 0;
}

void CMsgPack::Clear() {
	m_name = "";
	m_data.clear();
}

void CMsgPack::Print() {
	printf("msg: %s\n", m_name.c_str());
	map<string, string>::iterator it;
	for(it = m_data.begin(); it != m_data.end(); it++) {
		printf("%s: %s\n", it->first.c_str(), it->second.c_str());
		//cout << it->first << ": " << it->second << endl;
	}
}

string CMsgPack::GenBoundary() {
	char boundary[13] = "|YTAZFIREUH|";

	srand(time(NULL));
	while(true) {
		int found = 0;
		if(m_name.find(boundary) != string::npos) {
			found = 1;
		}
		if(!found) {
			map<string, string>::iterator it;
			for(it = m_data.begin(); it != m_data.end(); it++) {
				if(it->first.find(boundary) != string::npos) {
					found = 1;
					break;
				}
				if(it->second.find(boundary) != string::npos) {
					found = 1;
					break;
				}
			}
		}

		if(!found) {
			break;
		}

		int i;
		for(i = 1; i <= 10; i++) {
			boundary[i] = 'A' + rand()%26;
		}
	}

	return boundary;
}
