#pragma once
#include <string>
using namespace std;

struct Server 
{
	string strProtocol; // Э��
	string strHost;		// ������
	string strPort;			// �˿ں�
};

// Ӧ�������࣬�������ļ����ж�д
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

