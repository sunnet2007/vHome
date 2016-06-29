#pragma once
#include <string>
using namespace std;

struct Server 
{
	string strProtocol; // 协议
	string strHost;		// 主机号
	string strPort;			// 端口号
};

// 应用配置类，对配置文件进行读写
class CAppConfig
{
public:
	CAppConfig(void);
	~CAppConfig(void);
public:
	static void GetServer(Server& server);
	static string NOS_FENXI;
	static string NOS_TEACHERS; 
	static string NOS_TEACHER_EXCHANGEINFO;
	static string NOS_SENDYZM;
	static string NOS_REG;
	static string NOS_LOGIN;
	static string NOS_MODIFYPASSWORD;
	static string ADDATTENTION;
	static string USER_TEACHER_INFO;
	static string DELATTENTION;
	static string NOS_TOPTEACHER;
	static string NOS_C_STARTEACHER;
};

