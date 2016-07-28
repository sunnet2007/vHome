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
	static string NOS_C_LOGIN;
	static string NOS_MODIFYPASSWORD;
	static string ADDATTENTION;
	static string USER_TEACHER_INFO;
	static string DELATTENTION;
	static string NOS_TOPTEACHER;
	static string NOS_C_STARTEACHER;
	static string NOS_SENDZHYZM;
	static string NOS_GETPWD;
	static string NOS_ACTIVITY;
	static string MY_ACTIVITY;
	static string NOS_ACTIVITY_INFO;
	static string NOS_ACTIVITY_CONTENT;
	static string ENROLL_ACTIVITY;
	static string NOS_C_TEACHERS_INFO;
	static string NOS_TEACHER_ARTICLEINFO;
	static string NOS_TEACHER_ALLARTICLE;
	static string NOS_TEACHER_PRERELEASE;
	static string NOS_TEACHER_RELEASE;
	static string NOS_TEACHER_INDEX;
	static string NOS_USERIMAGE;
	static string NOS_USERINFO_VIEW;
	static string NOS_USERINFO_MODI;
	static string NOS_TEACHER_VIEW;
	static string NOS_TEACHER_MODI;
	static string NOS_TEST;
	static string NOS_TEACHER_HEADING;
	static string NOS_UPLOADARTICLEIMG;
	static string UI_FILE_PATH;
	static string NOS_ABOUTUS;
};

